#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_SENTENCE_H_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_SENTENCE_H_

#include <vector>
#include <string>
#include <ostream>
#include <optional>
#include <tuple>

#include "orword.hpp"
#include "id.hpp"

namespace parser::constructs {
	class Sentence;
	class CapturingSentence;
}

#include "unfolded.hpp"

namespace parser::constructs {
	class Sentence {
		Id m_id;
		std::vector<OrWord> m_orWords;
	public:
		Sentence(const std::vector<OrWord>& orWords, const std::optional<Id>& id);
		std::vector<UnfoldedSentence> unfolded() const;

		friend std::ostream& operator<< (std::ostream& stream, const Sentence& sentence);
	};
	
	class CapturingSentence {
		Id m_id;
		std::vector<OrWord> m_orWordsBefore;
		std::vector<OrWord> m_orWordsAfter;
	public:
		CapturingSentence(const std::vector<OrWord>& orWordsBefore, const std::vector<OrWord>& orWordsAfter, const std::optional<Id>& id);
		std::vector<UnfoldedCapturingSentence> unfolded() const;

		friend std::ostream& operator<< (std::ostream& stream, const CapturingSentence& sentence);
	};
}

#endif