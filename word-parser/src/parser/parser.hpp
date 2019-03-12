#ifndef _WORD_PARSER_PARSER_PARSER_HPP_
#define _WORD_PARSER_PARSER_PARSER_HPP_

#include "sentence.hpp"

#include <tuple>

namespace parser {
	constexpr int minimumRequiredScore = 15;

	class Parser {
		std::vector<Sentence> m_sentences;
		std::vector<CapturingSentence> m_capturingSentences;
		const std::string m_idWhenInvalid, m_codeWhenInvalid;

		std::pair<const Sentence*, int> getHighestScoreSentence(const std::vector<std::string>& insertedWords) const;
		std::tuple<const CapturingSentence*, size_t, size_t, int> getHighestScoreCapturingSentence(const std::vector<std::string>& insertedWords) const;

	public:
		Parser(const std::vector<Sentence>& sentences, const std::vector<CapturingSentence>& capturingSentences, const std::string& idWhenInvalid, const std::string& codeWhenInvalid);
		
		std::unique_ptr<ParsedSentenceBase> parse(const std::vector<std::string>& insertedWords, const std::vector<std::string>& insertedWordsLowercase) const;

		void add(const Sentence& sentence);
		void add(const CapturingSentence& capturingSentence);
	};
}

#endif