#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_SENTENCE_H_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_SENTENCE_H_

#include <vector>
#include <string>
#include <ostream>

#include "orword.h"

namespace parser::constructs {
	class Sentence {
		std::vector<OrWord> m_orWords;
	public:
		Sentence(const std::vector<OrWord>& orWords);
		std::vector<std::vector<std::string>> unfolded() const;

		friend std::ostream& operator<< (std::ostream& stream, const Sentence& sentence);
	};
	
	class CapturingSentence {
		std::vector<OrWord> m_orWordsBefore;
		std::vector<OrWord> m_orWordsAfter;
	public:
		CapturingSentence(const std::vector<OrWord>& orWordsBefore, const std::vector<OrWord>& orWordsAfter);
		std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> unfolded() const;

		friend std::ostream& operator<< (std::ostream& stream, const CapturingSentence& sentence);
	};
}

#endif