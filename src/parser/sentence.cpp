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
		
		auto word = m_words.begin();
		auto compareWord = compareWords.begin();

		for (; word != m_words.end() && compareWord != compareWords.end(); ++word) {
			auto foundWord = std::find(compareWord, compareWords.end(), *word);

			if (foundWord == compareWords.end()) {
				foundAllWords = false;
			}
			else {
				points += pointsFoundWord;

				if (foundWord != compareWord) {
					exactMatch = false;
					points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(foundWord - compareWord));
				}

				compareWord = foundWord + 1;
			}
		}

		if (word != m_words.end()) {
			exactMatch = false;
			points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(m_words.end() - word));
		}
		else if (compareWord != compareWords.end()) {
			exactMatch = false;
			points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(compareWords.end() - compareWord));
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