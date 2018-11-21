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

	Application::Encoding Application::toEncoding(const string& str) {
		if (str == "8bithex")
			return hex8bit;
		else if (str == "16bithex")
			return hex16bit;
		return Encoding{-1};
	}
	Application::Format Application::toFormat(const string& str) {
		if (str == "json")
			return json;
		return Format{-1};
	}


	unique_ptr<std::ostream> Application::nonDefualtOutput = nullptr;
	unique_ptr<std::ostream> Application::nonDefaultLogs = nullptr;


	stypox::ArgParser Application::initialArgs{
		"word-parser",
		{
			{"help", "prints the help screen and exits", {"-?", "-h", "--help"}}
		}, {

		}, {

		}, {
			{"output", "the output file (defaults to stdout)", {"-o=", "--output="}, "stdout"},
			{
				"encoding",
				"input encoding (required, values: 8bithex (2 hexadecimal 0-f), 16bithex (4 hexadecimal 0-f))",
				{"-e=", "--encoding="},
				{},
				[](string s) { return s == "8bithex" || s == "16bithex"; }
			},
			{"format", "output format (values: json (default))", {"-f=", "--format="}, "json", [](string s) { return s == "json"; }},
			{"logs", "the file to save logs in (when openable) (stdout is valid)", {"-l=", "--logs="}}
		}
	};
	stypox::ArgParser Application::currentArgs{
		"word-parser",
		{
			{"help", "prints the help screen and exits", {"-?", "-h", "--help"}},
			{"quit", "quit", {"-q", "--quit"}}
		}, {

		}, {

		}, {
			{"sentences", "replaces the precompiled sentences (format: sectionIdWhenInvalid,codeWhenInvalidHex;sectionId,sentenceId,word-word,codeHex;sectionId,sentenceId,word-word,word-word,codeHex;...)", {"-s=", "--sentences="}},
			{"inserted", "inserted words (required, format: as chosen on startup)", {"-i=", "--inserted="}, {}}
		},
		false
	};
	std::ostream* Application::output = nullptr;
	std::ostream* Application::logs = nullptr;


	void openFile(const string& filename, std::ostream*& file, unique_ptr<std::ostream>& nonDefaultFile) {
		if (filename == "stdout") {
			file = &std::cout;
		}
		else {
			unique_ptr<std::ofstream> openedFile{new std::ofstream{filename, std::ios::binary}};
			if (openedFile->is_open()) {
				nonDefaultFile = std::move(openedFile);
				file = nonDefaultFile.get();
			}
			else {
				file = nullptr;
			}
		}
	}

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
			initialArgs.parse(argc, argv);
			
			openFile(initialArgs.getText("logs"), logs, nonDefaultLogs);
			if (initialArgs.getBool("help")) {
				std::cout << initialArgs.help();
				exit(0);
			}
			
			initialArgs.validate();
		}
		catch (const std::runtime_error& e) {
			if (logs)
				*logs << "Error while parsing initial arguments: " << e.what() << std::flush;
			exit(1);
		}

		openFile(initialArgs.getText("output"), output, nonDefualtOutput);
		if (!output) {
			if (logs)
				*logs << "Error while parsing initial arguments: output file does not exist: " << initialArgs.getText("output") << std::flush;
			exit(1);
		}
	}
	void Application::parseCurrentArgs(const vector<string>& args) {
		try {
			currentArgs.reset();
			currentArgs.parse(args);

			if (currentArgs.getBool("help")) {
				std::cout << currentArgs.help();
				exit(0);
			}
			else if (currentArgs.getBool("quit")) {
				exit(0);
			}

			currentArgs.validate();
		}
		catch (const std::runtime_error& e) {
			if (logs)
				*logs << "Error while parsing current arguments: " << e.what() << std::flush;
			exit(1);
		}
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
		
		const Encoding encoding = toEncoding(initialArgs.getText("encoding"));
		const Format format = toFormat(initialArgs.getText("format"));
		while (1) {
			parseCurrentArgs(getArgs());

			// determine which parser to use
			parser::Parser* parser;
			unique_ptr<parser::Parser> nonDefaultParser = nullptr;
			if (string sentences = currentArgs.getText("sentences"); sentences.empty()) {
				parser = &sentences_compiler_gen::parser;
			}
			else {
				nonDefaultParser = generateParser(sentences);
				parser = nonDefaultParser.get();
			}

			// decode inserted words
			vector<string> insertedWords, insertedWordsLowercase;
			switch (encoding) {
			case hex8bit: {
				auto words = extractWords(fromHexTo8bit(currentArgs.getText("inserted")));
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