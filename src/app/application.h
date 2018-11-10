#ifndef _SENTENCES_COMPILER_APP_APPLICATION_H_
#define _SENTENCES_COMPILER_APP_APPLICATION_H_

#include <argparser.h>

namespace app {
	class Application {
	public:
		static stypox::ArgParser args;

		static int run(int argc, char const *argv[]);
	};
}

#endif