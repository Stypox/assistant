#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_UNFOLDED_HPP_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_UNFOLDED_HPP_

#include <vector>

namespace parser::constructs {
	struct UnfoldedSentence;
	struct UnfoldedCapturingSentence;
}

#include "id.hpp"
#include "sentence.hpp"

namespace parser::constructs {
	struct UnfoldedSentence {
		Id id;
		std::vector<std::string> words;

		bool operator<(const UnfoldedSentence& other) const;
	};

	struct UnfoldedCapturingSentence {
		Id id;
		std::vector<std::string> wordsBefore;
		std::vector<std::string> wordsAfter;

		bool operator<(const UnfoldedCapturingSentence& other) const;
	};

	std::vector<UnfoldedSentence> unfolded(const std::vector<Sentence>& sentences);
	std::vector<UnfoldedCapturingSentence> unfolded(const std::vector<CapturingSentence>& sentences);
}

#endif