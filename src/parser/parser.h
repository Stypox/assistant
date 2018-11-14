#ifndef _VOICE_ASSISTANT_PARSER_PARSER_H_
#define _VOICE_ASSISTANT_PARSER_PARSER_H_

#include "sentence.h"

#include <tuple>

namespace parser {
	constexpr int minimumRequiredScore = 15;

	class Parser {
		std::vector<Sentence> m_sentences;
		std::vector<CapturingSentence> m_capturingSentences;
		const std::string m_idWhenInvalid, m_codeWhenInvalid;

		std::pair<const Sentence*, int> getHighestScoreSentence(const std::vector<std::string>& words) const;
		std::tuple<const CapturingSentence*, std::vector<std::string>, int> getHighestScoreCapturingSentence(const std::vector<std::string>& words) const;

	public:
		Parser(const std::vector<Sentence>& sentences, const std::vector<CapturingSentence>& capturingSentences, const std::string& idWhenInvalid, const std::string& codeWhenInvalid);
		
		std::unique_ptr<ParsedSentenceBase> parse(const std::vector<std::string>& words) const;

		void add(const Sentence& sentence);
		void add(const CapturingSentence& capturingSentence);
	};
}

#endif