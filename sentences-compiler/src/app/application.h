#ifndef _SENTENCES_COMPILER_APP_APPLICATION_H_
#define _SENTENCES_COMPILER_APP_APPLICATION_H_

#include <stypox/argparser.h>

namespace app {
	class Application {
	public:
		static bool help, pretty, verbose;
		static std::string input, output, language;

		static int run(int argc, char const *argv[]);
	};
}

#endif