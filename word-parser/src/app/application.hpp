#ifndef _WORD_PARSER_APP_APPLICATION_H_
#define _WORD_PARSER_APP_APPLICATION_H_

#include <stypox/argparser.hpp>
#include <memory>
#include <ostream>

#include "../parser/parser.hpp"

namespace app {
	class Application {
	public:
		enum Encoding : char {
			hex8bit,
			hex16bit,
		};
		enum Format : char {
			json,
		};

	private:
		static void parseInitialArgs(int argc, char const *argv[]);
		static std::pair<std::string, std::string> parseCurrentArgs(const std::vector<std::string>& cliArgs);
		static std::vector<std::string> getArgs();

		static std::string fromHexTo8bit(const std::string& hex);
		static std::wstring fromHexTo16bit(const std::string& hex);

		static std::pair<std::vector<std::string>, std::vector<std::string>> extractWords(std::string input);
		static std::unique_ptr<parser::Parser> generateParser(const std::string& customSentences);

	public:
		static std::unique_ptr<std::ostream> nonDefualtOutput;
		static std::unique_ptr<std::ostream> nonDefaultLogs;

		static bool help;
		static std::ostream* output, * logs;
		static Encoding encoding;
		static Format format;

		static int run(int argc, char const *argv[]);
	};
}

#endif