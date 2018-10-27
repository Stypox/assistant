#include "parser.h"

namespace parser {
	std::pair<const Sentence*, bool> Parser::getHighestScoreSentence(const std::vector<std::string>& words) const {
		const Sentence* bestSentence = nullptr;
		int maxScoreSoFar = std::numeric_limits<int>::min();

		for (auto&& sentence : m_sentences) {
			int score = sentence.score(words);
			if (score > maxScoreSoFar) {
				maxScoreSoFar = score;
				bestSentence = &sentence;
			}
		}

		return {bestSentence, maxScoreSoFar > minimumRequiredScore};
	}

	void Parser::parse(const std::vector<std::string>& words) const {
		auto [bestSentence, scoreAboveMinimum] = getHighestScoreSentence(words);

		if (bestSentence && scoreAboveMinimum)
			bestSentence->exec();
		// TODO execute code for sentences that are not understood
	}

	void Parser::add(const Sentence& sentence) {
		m_sentences.push_back(sentence);
	}
}