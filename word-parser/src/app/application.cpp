#include "application.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "../../sentences.cpp"

namespace app {
	using std::string;
	using std::vector;
	using std::unique_ptr;
	using std::pair;
	using stypox::args;


	unique_ptr<std::ostream> Application::nonDefualtOutput = nullptr;
	unique_ptr<std::ostream> Application::nonDefaultLogs = nullptr;

	bool Application::help = false;
	std::ostream* Application::output = &std::cout,
		* Application::logs = nullptr;
	Application::Encoding Application::encoding;
	Application::Format Application::format = Application::json;


	std::ostream* openFile(const string& filename, unique_ptr<std::ostream>& nonDefaultFile) {
		if (filename == "stdout") {
			return &std::cout;
		}
		else {
			unique_ptr<std::ofstream> openedFile{new std::ofstream{filename, std::ios::binary}};
			if (openedFile->is_open()) {
				nonDefaultFile = std::move(openedFile);
				return nonDefaultFile.get();
			}
			else {
				return nullptr;
			}
		}
	}

	stypox::ArgParser initialArgParser{
		std::make_tuple(
			stypox::SwitchOption{"help", Application::help, args("-?", "-h", "--help"), "prints the help screen and exits"},
			stypox::ManualOption{"output", Application::output, args("-o=", "--output="),
				"the output file (defaults to stdout)", [&](const std::string_view& s) {
				std::ostream* output = openFile(string{s}, Application::nonDefualtOutput);
				if (!output) throw std::runtime_error{"Invalid output file \"" + string{s} + "\""};
				return output;
			}},
			stypox::ManualOption{"logs", Application::logs, args("-l=", "--logs="),
				"the file to save logs in (when openable) (stdout is valid)",
				[&](const std::string_view& s) {
				return openFile(string{s}, Application::nonDefaultLogs);
			}},
			stypox::ManualOption{"encoding", Application::encoding, args("-e=", "--encoding="),
				"input encoding (values: hex8bit (2 hexadecimal 0-f), hex16bit (4 hexadecimal 0-f))",
				[](const std::string_view& s) {
				if (s == "hex8bit")
					return Application::hex8bit;
				else if (s == "hex16bit")
					return Application::hex16bit;
				else
					throw std::runtime_error{"Invalid encoding \"" + string{s} + "\""};
			}, true},
			stypox::ManualOption{"format", Application::format, args("-f=", "--format="),
				"output format (values: json (default))",
				[](const std::string_view& s) {
				if (s == "json")
					return Application::json;
				else
					throw std::runtime_error{"Invalid format \"" + string{s} + "\""};
			}}
		),
		"word-parser"
	};
	

	vector<string> splitAtSpaces(const string& str) {
		// counts two or more spaces as only one, thus does not generate empty strings
		vector<string> result;
		auto begin = str.begin();

		while (1) {
			while (begin != str.end() && isspace(*begin))
				++begin;
			if (begin == str.end())
				break;

			auto found = std::find_if(begin, str.end(), isspace);
			result.emplace_back(begin, found);

			begin = found;
		}

		return result;
	}
	vector<string> splitEvery(const string& str, char ch) {
		// counts N chars N chars, thus generating empty strings
		vector<string> result;
		auto begin = str.begin();

		while (1) {
			if (begin >= str.end())
				break;

			auto found = std::find(begin, str.end(), ch);
			result.emplace_back(begin, found);

			begin = found + 1;
		}

		return result;
	}

	void Application::parseInitialArgs(int argc, char const *argv[]) {
		try {
			initialArgParser.parse(argc, argv);
			
			if (help) {
				std::cout << initialArgParser.help();
				exit(0);
			}
			
			initialArgParser.validate();
		}
		catch (const std::runtime_error& e) {
			if (logs)
				*logs << "Error while parsing initial arguments: " << e.what() << std::flush;
			exit(1);
		}
	}
	pair<string, string> Application::parseCurrentArgs(const vector<string>& cliArgs) {
		static bool currentHelp = false,
			currentQuit = false;
		static string inserted{},
			parserSentences{};

		static stypox::ArgParser currentArgParser{
			std::make_tuple(
				stypox::SwitchOption{"help", currentHelp, args("-?", "-h", "--help"), "prints the help screen and exits"},
				stypox::SwitchOption{"quit", currentQuit, args("-q", "--quit"), "quit"},
				stypox::Option{"sentences", parserSentences, args("-s=", "--sentences="),
					"replaces the precompiled sentences (format: sectionIdWhenInvalid,codeWhenInvalidHex;sectionId,sentenceId,word-word,codeHex;sectionId,sentenceId,word-word,word-word,codeHex;...)"},
				stypox::Option{"inserted", inserted, args("-i=", "--inserted="), "inserted words (required, format: as chosen on startup)", true}
			),
			"word-parser"
		};

		// default values for the parser
		parserSentences = "";
		currentArgParser.reset();

		try {
			currentArgParser.parse(cliArgs.begin(), cliArgs.end(), false);

			if (currentHelp) {
				std::cout << currentArgParser.help();
				exit(0);
			}
			else if (currentQuit) {
				exit(0);
			}

			currentArgParser.validate();
		}
		catch (const std::runtime_error& e) {
			if (logs)
				*logs << "Error while parsing current arguments: " << e.what() << std::flush;
			exit(1);
		}

		return {inserted, parserSentences};
	}
	vector<string> Application::getArgs() {
		string line;
		std::getline(std::cin, line);

		return splitAtSpaces(line);
	}

	unsigned int fromHex(char ch) {
		if (ch >= '0' && ch <= '9')
			return ch - '0';
		else if (ch >= 'A' && ch <= 'Z')
			return ch - 'A' + 10;
		else
			return ch - 'a' + 10;
	}
	string Application::fromHexTo8bit(const string& hex) {
		if (hex.size() % 2 != 0) {
			if (logs)
				*logs << "Malformed 8bit hexadecimal\n" << std::flush;
			exit(1);
		}

		string result;
		result.reserve(hex.size() / 2);
		for (size_t pos = 0; pos < hex.size(); pos += 2)
			result.push_back(16*fromHex(hex[pos]) + fromHex(hex[pos + 1]));

		return result;
	}
	std::wstring Application::fromHexTo16bit(const string& hex) {
		if (hex.size() % 4 != 0) {
			if (logs)
				*logs << "Malformed 16bit hexadecimal\n" << std::flush;
			exit(1);
		}

		std::wstring result;
		result.reserve(hex.size() / 2);
		for (size_t pos = 0; pos < hex.size(); pos += 2)
			result.push_back(4096*fromHex(hex[pos]) + 256*fromHex(hex[pos + 1]) + 16*fromHex(hex[pos + 2]) + fromHex(hex[pos + 3]));

		return result;
	}

	pair<vector<string>, vector<string>> Application::extractWords(string input) {
		for (auto&& ch : input) {
			switch (ch) {
			case '"': case '(': case ')': case '[':
			case ']': case ',': case ':': case ';':
			case '.': case '-': case '_': case '!':
			case '?': case '\\': case '/':
				ch = ' ';
			}
		}
		auto insertedWords = splitAtSpaces(input);

		for (auto&& ch : input) {
			if (ch >= 'A' && ch <= 'Z')
				ch -= ('A' - 'a');
		}
		auto insertedWordsLowercase = splitAtSpaces(input);

		return {insertedWords, insertedWordsLowercase};
	}
	unique_ptr<parser::Parser> Application::generateParser(const string& input) {
		auto sentences = splitEvery(input, ';');
		
		auto sentence0Split = splitEvery(sentences[0], ',');
		if (sentence0Split.size() != 2) {
			if (logs)
				*logs << "Invalid sentences: " << input
					<< "\nAt section 1: \n" << sentences[0] << "\n" << std::flush;
			exit(1);
		}
		string idWhenInvalid = sentence0Split[0];
		string codeWhenInvalid = fromHexTo8bit(sentence0Split[1]);

		vector<parser::Sentence> resSentences;
		vector<parser::CapturingSentence> resCapturingSentences;
		for (auto sentence = sentences.begin() + 1; sentence != sentences.end(); ++sentence) {
			auto sentenceSplit = splitEvery(*sentence, ',');
			if (sentenceSplit.size() == 4)
				resSentences.emplace_back(sentenceSplit[0], sentenceSplit[1], splitEvery(sentenceSplit[2], '-'), fromHexTo8bit(sentenceSplit[3]));
			else if (sentenceSplit.size() == 5)
				resCapturingSentences.emplace_back(sentenceSplit[0], sentenceSplit[1], splitEvery(sentenceSplit[2], '-'), splitEvery(sentenceSplit[3], '-'), fromHexTo8bit(sentenceSplit[4]));
			else {
				if (logs)
					*logs << "Invalid sentences: " << input
						<< "\nAt section " << sentence - sentences.begin() + 1 << ": " << *sentence << std::flush;
				exit(1);
			}
		}

		return unique_ptr<parser::Parser>{new parser::Parser{resSentences, resCapturingSentences, idWhenInvalid, codeWhenInvalid}};
	}

	int Application::run(int argc, char const *argv[]) {
		parseInitialArgs(argc, argv);
		
		while (1) {
			auto [inserted, parserSentences] = parseCurrentArgs(getArgs());

			// determine which parser to use
			parser::Parser* parser;
			unique_ptr<parser::Parser> nonDefaultParser = nullptr;
			if (parserSentences.empty()) {
				parser = &sentences_compiler_gen::parser;
			}
			else {
				nonDefaultParser = generateParser(parserSentences);
				parser = nonDefaultParser.get();
			}

			// decode inserted words
			vector<string> insertedWords, insertedWordsLowercase;
			switch (encoding) {
			case hex8bit: {
				auto words = extractWords(fromHexTo8bit(inserted));
				insertedWords = words.first;
				insertedWordsLowercase = words.second;
				break;
			}
			case hex16bit: {
				// TODO
				break;
			}
			}

			// parse inserted words
			unique_ptr<parser::ParsedSentenceBase> parsedInput = parser->parse(insertedWords, insertedWordsLowercase);
			
			// output parsed input
			if (logs){
				parsedInput->log(*logs);
				logs->flush();
			}
			switch (format) {
			case json:
				parsedInput->json(*output);
				output->flush();
				break;
			}
		}
	}
}