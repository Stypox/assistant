#ifndef _ASSISTANT_APP_APPLICATION_H_
#define _ASSISTANT_APP_APPLICATION_H_

#include <argparser.h>
#include <memory>
#include <ostream>

#include "../parser/parser.h"

namespace app {
	class Application {
		enum Encoding : char {
			hex8bit,
			hex16bit,
		};
		static Encoding toEncoding(const std::string& str);
		enum Format : char {
			json,
		};
		static Format toFormat(const std::string& str);

		static void parseInitialArgs(int argc, char const *argv[]);
		static void parseCurrentArgs(const std::vector<std::string>& args);
		static std::vector<std::string> getArgs();

		static std::string fromHexTo8bit(const std::string& hex);
		static std::wstring fromHexTo16bit(const std::string& hex);

		static std::vector<std::string> parseWords(std::string sentence);
		static std::unique_ptr<parser::Parser> parseSentences(const std::string& sentences);

	public:
		static stypox::ArgParser initialArgs;
		static stypox::ArgParser currentArgs;
		static std::ostream* output;
		static std::ostream* logs;

		static int run(int argc, char const *argv[]);
	};
}

#endif