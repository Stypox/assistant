#ifndef _VOICE_ASSISTANT_EXEC_EXECUTER_H_
#define _VOICE_ASSISTANT_EXEC_EXECUTER_H_

#include <string>

namespace exec {
	void initialize();
	int finalize();
	int checkSignals();

	int execute(const std::string& code);
}

#endif