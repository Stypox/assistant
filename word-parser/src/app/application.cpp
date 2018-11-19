#include "application.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "../../sentences.cpp"

namespace app {
	Application::Encoding Application::toEncoding(const std::string& str) {
		if (str == "8bithex")
			return hex8bit;
		else if (str == "16bithex")
			return hex16bit;
		return Encoding{-1};
	}
	Application::Format Application::toFormat(const std::string& str) {
		if (str == "json")
			return json;
		return Format{-1};
	}


	std::unique_ptr<std::ostream> Application::nonDefualtOutput = nullptr;
	std::unique_ptr<std::ostream> Application::nonDefaultLogs = nullptr;


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
				[](std::string s) { return s == "8bithex" || s == "16bithex"; }
			},
			{"format", "output format (values: json (default))", {"-f=", "--format="}, "json", [](std::string s) { return s == "json"; }},
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


	void openFile(const std::string& filename, std::ostream*& file, std::unique_ptr<std::ostream>& nonDefaultFile) {
		if (filename == "stdout") {
			file = &std::cout;
		}
		else {
			std::unique_ptr<std::ofstream> openedFile{new std::ofstream{filename, std::ios::binary}};
			if (openedFile->is_open()) {
				nonDefaultFile = std::move(openedFile);
				file = nonDefaultFile.get();
			}
			else {
				file = nullptr;
			}
		}
	}

	std::vector<std::string> splitAtSpaces(const std::string& str) {
		// counts two or more spaces as only one, thus does not generate empty strings
		std::vector<std::string> result;
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
	std::vector<std::string> splitEvery(const std::string& str, char ch) {
		// counts N chars N chars, thus generating empty strings
		std::vector<std::string> result;
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
	void Application::parseCurrentArgs(const std::vector<std::string>& args) {
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
	std::vector<std::string> Application::getArgs() {
		std::string line;
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
	std::string Application::fromHexTo8bit(const std::string& hex) {
		if (hex.size() % 2 != 0) {
			if (logs)
				*logs << "Malformed 8bit hexadecimal\n" << std::flush;
			exit(1);
		}

		std::string result;
		result.reserve(hex.size() / 2);
		for (size_t pos = 0; pos < hex.size(); pos += 2)
			result.push_back(16*fromHex(hex[pos]) + fromHex(hex[pos + 1]));

		return result;
	}
	std::wstring Application::fromHexTo16bit(const std::string& hex) {
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

	std::vector<std::string> Application::parseWords(std::string sentence) {
		for (auto&& ch : sentence) {
			if (ch >= 'A' && ch <= 'Z') {
				ch -= ('A' - 'a');
			}
			else {
				switch (ch) {
				case '"': case '(': case ')': case '[':
				case ']': case ',': case ':': case ';':
				case '.': case '-': case '_': case '!':
				case '?': case '\\': case '/':
					ch = ' ';
				}
			}
		}

		return splitAtSpaces(sentence);
	}
	std::unique_ptr<parser::Parser> Application::parseSentences(const std::string& sentences) {
		auto splitSentences = splitEvery(sentences, ';');
		
		auto firstSectionSplit = splitEvery(splitSentences[0], ',');
		if (firstSectionSplit.size() != 2) {
			if (logs)
				*logs << "Invalid sentences: " << sentences
					<< "\nAt section 1: \n" << splitSentences[0] << "\n" << std::flush;
			exit(1);
		}
		std::string idWhenInvalid = firstSectionSplit[0];
		std::string codeWhenInvalid = fromHexTo8bit(firstSectionSplit[1]);

		std::vector<parser::Sentence> resSentences;
		std::vector<parser::CapturingSentence> resCapturingSentences;
		for (auto section = splitSentences.begin() + 1; section != splitSentences.end(); ++section) {
			auto sectionSplit = splitEvery(*section, ',');
			if (sectionSplit.size() == 4)
				resSentences.emplace_back(sectionSplit[0], sectionSplit[1], splitEvery(sectionSplit[2], '-'), fromHexTo8bit(sectionSplit[3]));
			else if (sectionSplit.size() == 5)
				resCapturingSentences.emplace_back(sectionSplit[0], sectionSplit[1], splitEvery(sectionSplit[2], '-'), splitEvery(sectionSplit[3], '-'), fromHexTo8bit(sectionSplit[4]));
			else {
				if (logs)
					*logs << "Invalid sentences: " << sentences
						<< "\nAt section " << section - splitSentences.begin() + 1 << ": " << *section << std::flush;
				exit(1);
			}
		}

		return std::unique_ptr<parser::Parser>{new parser::Parser{resSentences, resCapturingSentences, idWhenInvalid, codeWhenInvalid}};
	}

	int Application::run(int argc, char const *argv[]) {
		parseInitialArgs(argc, argv);
		
		const Encoding encoding = toEncoding(initialArgs.getText("encoding"));
		const Format format = toFormat(initialArgs.getText("format"));
		while (1) {
			parseCurrentArgs(getArgs());

			// determine which parser to use
			parser::Parser* parser;
			std::unique_ptr<parser::Parser> nonDefaultParser = nullptr;
			if (std::string sentences = currentArgs.getText("sentences"); sentences.empty()) {
				parser = &sentences_compiler_gen::parser;
			}
			else {
				nonDefaultParser = parseSentences(sentences);
				parser = nonDefaultParser.get();
			}

			// decode inserted words
			std::vector<std::string> insertedWords;
			switch (encoding) {
			case hex8bit:
				insertedWords = parseWords(fromHexTo8bit(currentArgs.getText("inserted")));
				break;
			case hex16bit:
				// TODO
				break;
			}

			// parse sentence
			std::unique_ptr<parser::ParsedSentenceBase> parsedSentence = parser->parse(insertedWords);
			
			// output sentence
			if (logs){
				parsedSentence->log(*logs);
				logs->flush();
			}
			switch (format) {
			case json:
				parsedSentence->json(*output);
				output->flush();
				break;
			}
		}
	}
}