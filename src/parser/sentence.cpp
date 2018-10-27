#include "sentence.h"

#include <algorithm>
#include <iostream>

namespace parser {
	Sentence::Sentence(const std::vector<std::string>& words, const std::string& code) :
		m_words{words}, m_code{code} {}

	int Sentence::score(const std::vector<std::string>& compareWords) const {
		int points = pointsAtBeginning;
		bool foundAllWords = true,
			exactMatch = true;
		auto wordsBegin = m_words.begin();


		for (auto compareWord = compareWords.begin(); compareWord != compareWords.end(); ++compareWord) {
			if (wordsBegin == m_words.end()) {
				foundAllWords = false;
				points += std::max(-pointsFoundWord, static_cast<int>(compareWords.end() - compareWord) * pointsWordsInMiddle);
				break;
			}

			auto foundWord = std::find(wordsBegin, m_words.end(), *compareWord);

			if (foundWord == m_words.end()) {
				foundAllWords = false;
			}
			else {
				points += pointsFoundWord;

				if (foundWord != wordsBegin) {
					exactMatch = false;
					points += std::max(-pointsFoundWord, static_cast<int>(foundWord - wordsBegin) * pointsWordsInMiddle);
				}

				wordsBegin = foundWord + 1;
			}
		}

		if (foundAllWords) {
			points += pointsFoundAllWords;
			if (exactMatch)
				points += pointsExactMatch;
		}

		return points;
	}
	void Sentence::exec() const {
		std::cout << m_words[0];
	}
}