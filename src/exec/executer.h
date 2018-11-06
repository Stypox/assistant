#ifndef _VOICE_ASSISTANT_EXEC_EXECUTER_H_
#define _VOICE_ASSISTANT_EXEC_EXECUTER_H_

#include <string>
#include <vector>

namespace exec {
	constexpr const char* pyAssistantResponseClass =
		"class AssistantResponse:\n"
		"\tdef __init__(self, received_words, captured_words = None, sentence_words = None, sentence_words_before = None, sentence_words_after = None):\n"
		"\t\tself.received_words = received_words\n"
		"\t\tself.captured_words = captured_words\n"
		"\t\tself.sentence_words = sentence_words\n"
		"\t\tself.sentence_words_before = sentence_words_before\n"
		"\t\tself.sentence_words_after = sentence_words_after\n"
		"\t\tself.is_capturing_sentence = (sentence_words is None and sentence_words_before is not None and sentence_words_after is not None)\n"
		"\t\tself.understood = (sentence_words is None and sentence_words_before is None and sentence_words_after is None)\n";
	
	constexpr const char* assistantResponseName = "assistant_response";

	void initialize();
	int finalize();
	int checkSignals();

	int execute(const std::string& code);

	std::string buildArray(const std::vector<std::string>& words);
	std::string buildAssistantResonse(std::string name, const std::vector<std::string>& receivedWords);
	std::string buildAssistantResonse(std::string name, const std::vector<std::string>& receivedWords,
														const std::vector<std::string>& sentenceWords);
	std::string buildAssistantResonse(std::string name, const std::vector<std::string>& receivedWords,
														const std::vector<std::string>& capturedWords,
														const std::vector<std::string>& sentenceWordsBefore,
														const std::vector<std::string>& sentenceWordsAfter);

	std::string addPredef(const std::string code);
}

#endif