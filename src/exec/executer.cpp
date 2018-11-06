#include "executer.h"
#include "Python.h"
#include "pyerrors.h"

namespace exec {
	using Words = const std::vector<std::string>&;

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

	std::string buildArray(Words words) {
		std::string result = "[";
		for (auto&& word : words) {
			result += '"';
			result.append(word);
			result.append("\",");
		}

		if (words.empty())
			result.append("]");
		else
			result.back() = ']';

		return result;
	}
	std::string buildAssistantResonse(std::string name, Words receivedWords) {
		name.append("=AssistantResponse(");
		name.append(buildArray(receivedWords));
		name.append(")\n");
		return name;
	}
	std::string buildAssistantResonse(std::string name, Words receivedWords, Words sentenceWords) {
		name.append("=AssistantResponse(");
		name.append(buildArray(receivedWords));
		name.append(",sentence_words=");
		name.append(buildArray(sentenceWords));
		name.append(")\n");
		return name;
	}
	std::string buildAssistantResonse(std::string name, Words receivedWords, Words capturedWords, Words sentenceWordsBefore, Words sentenceWordsAfter) {
		name.append("=AssistantResponse(");
		name.append(buildArray(receivedWords));
		name.append(",captured_words=");
		name.append(buildArray(capturedWords));
		name.append(",sentence_words_before=");
		name.append(buildArray(sentenceWordsBefore));
		name.append(",sentence_words_after=");
		name.append(buildArray(sentenceWordsAfter));
		name.append(")\n");
		return name;
	}
	std::string addPredef(const std::string code) {
		return pyAssistantResponseClass + code;
	}
}