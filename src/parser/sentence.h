#ifndef _VOICE_ASSISTANT_PARSER_SENTENCE_H_
#define _VOICE_ASSISTANT_PARSER_SENTENCE_H_

#include <vector>
#include <string>
#include <tuple>

namespace parser {
	class Sentence;
	class CapturingSentence;
}

#include "parsed_sentence.h"

namespace parser {
	constexpr int pointsAtBeginning = 0,
		pointsFoundWord = 3,
		pointsWordsInMiddle = -1,
		minPointsWordInMiddle = -10,
		pointsFoundAllWords = 8,
		pointsExactMatch = 6,
		pointsNoWordsCaptured = -5;
	
	constexpr const char* receivedWordsPyName = "received_words",
		* capturedWordsPyName = "captured_words",
		* sentenceWordsPyName = "sentence_words",
		* sentenceWordsBeforePyName = "sentence_words_before",
		* sentenceWordsAfterPyName = "sentence_words_after";

	class Sentence {
		const std::string& m_id;
		const std::vector<std::string> m_words;
		const std::string& m_code;
	public:
		Sentence(const std::string& id, const std::vector<std::string>& words, const std::string& code);

		int score(const std::vector<std::string>& compareWords) const;
		void exec(const std::vector<std::string>& words) const;

		friend class ParsedSentence;
	};

	class CapturingSentence {
		const std::string& m_id;
		const std::vector<std::string> m_wordsBefore;
		const std::vector<std::string> m_wordsAfter;
		const std::string& m_code;

		std::tuple<int, std::vector<std::string>, bool, bool> scoreBefore(const std::vector<std::string>& compareWords) const;
		std::tuple<int, std::vector<std::string>, bool, bool> scoreAfter(const std::vector<std::string>& compareWords) const;
	public:
		CapturingSentence(const std::string& id, const std::vector<std::string>& wordsBefore, const std::vector<std::string> wordsAfter, const std::string& code);

		std::pair<int, std::vector<std::string>> score(const std::vector<std::string>& compareWords) const;
		void exec(const std::vector<std::string>& words, const std::vector<std::string>& capturedWords) const;

		friend class ParsedCapturingSentence;
	};
}

#endif