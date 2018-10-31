#include "sentence.h"

#include <algorithm>
#include <iostream>

namespace parser {
	using std::string;
	using std::vector;
	using std::tuple;
	using std::pair;


	Sentence::Sentence(const vector<string>& words, const string& code) :
		m_words{words}, m_code{code} {}

	int Sentence::score(const vector<string>& compareWords) const {
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


	tuple<int, vector<string>, bool, bool> CapturingSentence::scoreBefore(const vector<string>& compareWords) const {
		if (m_wordsAfter.empty())
			return {0, compareWords, true, true};

		int points = 0;
		bool foundAllWords = true,
			exactMatch = true;

		auto word = m_wordsAfter.rbegin();
		auto compareWord = compareWords.rbegin();

		for (; word != m_wordsAfter.rend() && compareWord != compareWords.rend(); ++word) {
			auto foundWord = std::find(compareWord, compareWords.rend(), *word);

			if (foundWord == compareWords.rend()) {
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

		if (word != m_wordsAfter.rend()) {
			exactMatch = false;
			points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(m_wordsAfter.rend() - word));
		}
		if (compareWord == compareWords.rend()) {
			points += pointsMissingCapturingGroup;
		}

		return {points, {compareWords.begin(), compareWords.begin() + static_cast<int>(compareWords.rend() - compareWord)}, foundAllWords, exactMatch};
	}
	tuple<int, vector<string>, bool, bool> CapturingSentence::scoreAfter(const vector<string>& compareWords) const {
		if (m_wordsBefore.empty())
			return {0, compareWords, true, true};

		int points = 0;
		bool foundAllWords = true,
			exactMatch = true;

		auto word = m_wordsBefore.begin();
		auto compareWord = compareWords.begin();

		for (; word != m_wordsBefore.end() && compareWord != compareWords.end(); ++word) {
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

		if (word != m_wordsBefore.end()) {
			exactMatch = false;
			points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(m_wordsAfter.end() - word));
		}
		if (compareWord >= compareWords.end()) {
			points += pointsMissingCapturingGroup;
		}

		return {points, {compareWords.begin() + static_cast<int>(compareWords.end() - compareWord - 1), compareWords.end()}, foundAllWords, exactMatch};
	}

	CapturingSentence::CapturingSentence(const vector<string>& wordsBefore, const vector<string> wordsAfter, const string& code) :
		m_wordsBefore{wordsBefore}, m_wordsAfter{wordsAfter}, m_code{code} {}

	pair<int, vector<string>> CapturingSentence::score(const vector<string>& compareWords) const {
		int points = pointsAtBeginning;

		const auto& [pointsAfter, remainingWords, foundAllWordsAfter, exactMatchAfter] = scoreAfter(compareWords);
		points += pointsAfter;
		const auto& [pointsBefore, capturingGroup, foundAllWordsBefore, exactMatchBefore] = scoreBefore(remainingWords);
		points += pointsBefore;

		if (foundAllWordsAfter && foundAllWordsBefore) {
			points += pointsFoundAllWords;
			if (exactMatchAfter && exactMatchBefore)
				points += pointsExactMatch;
		}

		return {points, capturingGroup};
	}
	void CapturingSentence::exec() const {
		std::cout << m_code;
	}
}