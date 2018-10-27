#ifndef _VOICE_ASSISTANT_PARSER_PARSER_H_
#define _VOICE_ASSISTANT_PARSER_PARSER_H_

#include "sentence.h"

#include <tuple>

namespace parser {
	constexpr int minimumRequiredScore = 10;

	class Parser {
		std::vector<Sentence> m_sentences;
		const std::string m_codeWhenNotUnderstood;

		std::pair<const Sentence*, bool> getHighestScoreSentence(const std::vector<std::string>& words) const;

	public:
		Parser(const std::vector<Sentence> sentences, const std::string& codeWhenNotUnderstood) :
			m_sentences{sentences}, m_codeWhenNotUnderstood{codeWhenNotUnderstood} {}

		void parse(const std::vector<std::string>& words) const;

		void add(const Sentence& sentence);
	};
}

#endif