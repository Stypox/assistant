#ifndef _WORD_PARSER_PARSER_SENTENCE_HPP_
#define _WORD_PARSER_PARSER_SENTENCE_HPP_

#include <vector>
#include <string>
#include <tuple>

namespace parser {
	class Sentence;
	class CapturingSentence;
}

#include "parsed_sentence.hpp"

namespace parser {
	constexpr int pointsAtBeginning = 0,
		pointsFoundWord = 3,
		pointsUnrecognizedWord = -1,
		minPointsUnrecognizedWords = -10,
		pointsFoundAllWords = 8,
		pointsExactMatch = 6,
		pointsNoWordsCaptured = -5;

	class Sentence {
		const std::string m_sectionId, m_sentenceId;		
		const std::vector<std::string> m_words;
		const std::string m_code;
	public:
		Sentence(const std::string& sectionId, const std::string& sentenceId, const std::vector<std::string>& words, const std::string& code);

		int score(const std::vector<std::string>& insertedWords) const;

		friend class ParsedSentence;
	};

	class CapturingSentence {
		const std::string m_sectionId, m_sentenceId;
		const std::vector<std::string> m_wordsBefore;
		const std::vector<std::string> m_wordsAfter;
		const std::string m_code;

		std::tuple<int, size_t, bool, bool> scoreBefore(const std::vector<std::string>& insertedWords) const;
		std::tuple<int, size_t, bool, bool> scoreAfter(const std::vector<std::string>& insertedWords) const;
	public:
		CapturingSentence(const std::string& sectionId, const std::string& sentenceId, const std::vector<std::string>& wordsBefore, const std::vector<std::string> wordsAfter, const std::string& code);

		std::tuple<int, size_t, size_t> score(const std::vector<std::string>& insertedWords) const;

		friend class ParsedCapturingSentence;
	};
}

#endif