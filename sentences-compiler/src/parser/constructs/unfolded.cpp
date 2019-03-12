#include "unfolded.hpp"

#include <set>
#include <algorithm>

namespace parser::constructs {
	bool UnfoldedSentence::operator<(const UnfoldedSentence& other) const {
		return words < other.words;
	}
	bool UnfoldedCapturingSentence::operator<(const UnfoldedCapturingSentence& other) const {
		if (wordsBefore == other.wordsBefore)
			return wordsAfter < other.wordsAfter;
		else
			return wordsBefore < other.wordsBefore;
	}

	std::vector<UnfoldedSentence> unfolded(const std::vector<Sentence>& sentences) {
		std::set<UnfoldedSentence> removedDuplicates;
		for (auto&& sentence : sentences) {
			auto unfolded = sentence.unfolded();
			removedDuplicates.insert(unfolded.begin(), unfolded.end());
		}

		std::vector<UnfoldedSentence> result{removedDuplicates.begin(), removedDuplicates.end()};
		auto firstEmpty = std::remove_if(result.begin(), result.end(),
			[](const UnfoldedSentence& a) { return a.words.empty(); });
		result.erase(firstEmpty, result.end());

		return result;
	}

	std::vector<UnfoldedCapturingSentence> unfolded(const std::vector<CapturingSentence>& sentences) {
		std::set<UnfoldedCapturingSentence> removedDuplicates;
		for (auto&& sentence : sentences) {
			auto unfolded = sentence.unfolded();
			removedDuplicates.insert(unfolded.begin(), unfolded.end());
		}

		std::vector<UnfoldedCapturingSentence> result{removedDuplicates.begin(), removedDuplicates.end()};
		auto firstEmpty = std::remove_if(result.begin(), result.end(),
			[](const UnfoldedCapturingSentence& a) { return a.wordsBefore.empty() && a.wordsAfter.empty(); });
		result.erase(firstEmpty, result.end());

		return result;
	}
}