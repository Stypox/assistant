#ifndef _VOICE_ASSISTANT_APP_APPLICATION_H_
#define _VOICE_ASSISTANT_APP_APPLICATION_H_

#include <argparser.h>
#include <memory>
#include <ostream>

namespace app {
	class Application {
		static bool parseInitialArgs(int argc, char const *argv[]);
		
		static const stypox::ArgParser currentArgsInitializer;

	public:
		static stypox::ArgParser initialArgs;
		static stypox::ArgParser currentArgs;
		static std::unique_ptr<std::ostream> output;
		static std::unique_ptr<std::ostream> logs;

		static int run(int argc, char const *argv[]);
	};
}

#endif