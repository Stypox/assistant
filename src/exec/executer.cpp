#include "executer.h"
#include "Python.h"

namespace exec {
	void execute(const std::string& code) {
		Py_Initialize();
		PyRun_SimpleString(code.c_str());
		if (Py_FinalizeEx() < 0) {
			exit(120);
		}
	}
}