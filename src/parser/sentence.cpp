#include "sentence.h"

#include <algorithm>
#include <iostream>

namespace parser {
	using std::string;
	using std::vector;
	using std::tuple;
	using std::pair;


	Sentence::Sentence(const string& id, const vector<string>& words, const string& code) :
		m_id{id}, m_words{words},
		m_code{code} {}

	int Sentence::score(const vector<string>& insertedWords) const {
		int points = pointsAtBeginning;
		bool foundAllWords = true,
			exactMatch = true;
		
		auto word = m_words.begin();
		auto insertedWord = insertedWords.begin();

		for (; word != m_words.end() && insertedWord != insertedWords.end(); ++word) {
			auto foundWord = std::find(insertedWord, insertedWords.end(), *word);

			if (foundWord == insertedWords.end()) {
				foundAllWords = false;
			}
			else {
				points += pointsFoundWord;

				if (foundWord != insertedWord) {
					exactMatch = false;
					points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(foundWord - insertedWord));
				}

				insertedWord = foundWord + 1;
			}
		}

		if (word != m_words.end()) {
			exactMatch = false;
			points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(m_words.end() - word));
		}
		else if (insertedWord != insertedWords.end()) {
			exactMatch = false;
			points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(insertedWords.end() - insertedWord));
		}

		if (foundAllWords) {
			points += pointsFoundAllWords;
			if (exactMatch)
				points += pointsExactMatch;
		}

		return points;
	}


	tuple<int, vector<string>, bool, bool> CapturingSentence::scoreBefore(const vector<string>& insertedWords) const {
		if (m_wordsAfter.empty())
			return {0, insertedWords, true, true};

		int points = 0;
		bool foundAllWords = true,
			exactMatch = true;

		auto word = m_wordsAfter.rbegin();
		auto insertedWord = insertedWords.rbegin();

		for (; word != m_wordsAfter.rend() && insertedWord != insertedWords.rend(); ++word) {
			auto foundWord = std::find(insertedWord, insertedWords.rend(), *word);

			if (foundWord == insertedWords.rend()) {
				foundAllWords = false;
			}
			else {
				points += pointsFoundWord;

				if (foundWord != insertedWord) {
					exactMatch = false;
					points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(foundWord - insertedWord));
				}

				insertedWord = foundWord + 1;
			}
		}

		if (word != m_wordsAfter.rend()) {
			exactMatch = false;
			points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(m_wordsAfter.rend() - word));
		}
		if (insertedWord == insertedWords.rend()) {
			points += pointsNoWordsCaptured;
		}

		return {points, {insertedWords.begin(), insertedWords.begin() + static_cast<int>(insertedWords.rend() - insertedWord)}, foundAllWords, exactMatch};
	}
	tuple<int, vector<string>, bool, bool> CapturingSentence::scoreAfter(const vector<string>& insertedWords) const {
		if (m_wordsBefore.empty())
			return {0, insertedWords, true, true};

		int points = 0;
		bool foundAllWords = true,
			exactMatch = true;

		auto word = m_wordsBefore.begin();
		auto insertedWord = insertedWords.begin();

		for (; word != m_wordsBefore.end() && insertedWord != insertedWords.end(); ++word) {
			auto foundWord = std::find(insertedWord, insertedWords.end(), *word);

			if (foundWord == insertedWords.end()) {
				foundAllWords = false;
			}
			else {
				points += pointsFoundWord;

				if (foundWord != insertedWord) {
					exactMatch = false;
					points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(foundWord - insertedWord));
				}

				insertedWord = foundWord + 1;
			}
		}

		if (word != m_wordsBefore.end()) {
			exactMatch = false;
			points += std::max(minPointsWordInMiddle, pointsWordsInMiddle * static_cast<int>(m_wordsBefore.end() - word));
		}
		if (insertedWord == insertedWords.end()) {
			points += pointsNoWordsCaptured;
			return {points, {}, foundAllWords, exactMatch};
		}

		return {points, {insertedWords.begin() + static_cast<int>(insertedWord - insertedWords.begin()), insertedWords.end()}, foundAllWords, exactMatch};
	}

	CapturingSentence::CapturingSentence(const string& id, const vector<string>& wordsBefore, const vector<string> wordsAfter, const string& code) :
		m_id{id}, m_wordsBefore{wordsBefore},
		m_wordsAfter{wordsAfter}, m_code{code} {}

	pair<int, vector<string>> CapturingSentence::score(const vector<string>& insertedWords) const {
		int points = pointsAtBeginning;

		const auto& [pointsAfter, remainingWords, foundAllWordsAfter, exactMatchAfter] = scoreAfter(insertedWords);
		points += pointsAfter;
		const auto& [pointsBefore, capturedWords, foundAllWordsBefore, exactMatchBefore] = scoreBefore(remainingWords);
		points += pointsBefore;

		if (foundAllWordsAfter && foundAllWordsBefore) {
			points += pointsFoundAllWords;
			if (exactMatchAfter && exactMatchBefore)
				points += pointsExactMatch;
		}

		return {points, capturedWords};
	}
}