#ifndef _SENTENCES_COMPILER_APP_APPLICATION_HPP_
#define _SENTENCES_COMPILER_APP_APPLICATION_HPP_

#include <stypox/argparser.hpp>

namespace app {
	class Application {
	public:
		static bool help, pretty, verbose;
		static std::string input, output, language;

		static int run(int argc, char const *argv[]);
	};
}

#endif