#include "executer.h"
#include "Python.h"
#include "pyerrors.h"

namespace exec {
	void initialize() {
		Py_Initialize();
	}
	int finalize() {
		return Py_FinalizeEx();
	}
	int checkSignals() {
		return PyErr_CheckSignals();
	}

	int execute(const std::string& code) {
		return PyRun_SimpleString(code.c_str());
	}

	std::string buildArray(std::string name, const std::vector<std::string>& words) {
		name.append("=[");
		for (auto&& word : words) {
			name += '"';
			name.append(word);
			name.append("\",");
		}

		if (words.empty())
			name.append("]\n");
		else {
			name.back() = ']';
			name += '\n';
		}

		return name;
	}
}