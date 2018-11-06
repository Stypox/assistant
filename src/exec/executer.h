#ifndef _VOICE_ASSISTANT_EXEC_EXECUTER_H_
#define _VOICE_ASSISTANT_EXEC_EXECUTER_H_

#include <string>
#include <vector>

namespace exec {
	void initialize();
	int finalize();
	int checkSignals();

	int execute(const std::string& code);

	std::string buildArray(std::string name, const std::vector<std::string>& words);
}

#endif