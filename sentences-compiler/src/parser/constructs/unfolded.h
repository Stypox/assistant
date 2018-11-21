#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_UNFOLDED_H_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_UNFOLDED_H_

#include <vector>

namespace parser::constructs {
	struct UnfoldedSentence;
	struct UnfoldedCapturingSentence;
}

#include "id.h"
#include "sentence.h"

namespace parser::constructs {
	struct UnfoldedSentence {
		Id id;
		std::vector<std::string> words;

		UnfoldedSentence(const Id& _id, std::vector<std::string>&& _words);
		bool operator<(const UnfoldedSentence& other) const;
	};

	struct UnfoldedCapturingSentence {
		Id id;
		std::vector<std::string> wordsBefore;
		std::vector<std::string> wordsAfter;

		UnfoldedCapturingSentence(const Id& _id, std::vector<std::string>&& _wordsBefore, std::vector<std::string>&& _wordsAfter);
		bool operator<(const UnfoldedCapturingSentence& other) const;
	};

	std::vector<UnfoldedSentence> unfolded(const std::vector<Sentence>& sentences);
	std::vector<UnfoldedCapturingSentence> unfolded(const std::vector<CapturingSentence>& sentences);
}

#endif