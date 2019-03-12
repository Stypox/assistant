#include "sentence.hpp"

#include <algorithm>
#include <iostream>

namespace parser {
	using std::string;
	using std::vector;
	using std::tuple;
	using std::pair;


	Sentence::Sentence(const string& sectionId, const string& sentenceId, const vector<string>& words, const string& code) :
		m_sectionId{sectionId}, m_sentenceId{sentenceId},
		m_words{words}, m_code{code} {}

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
					points += std::max(minPointsUnrecognizedWords, pointsUnrecognizedWord * static_cast<int>(foundWord - insertedWord));
				}

				insertedWord = foundWord + 1;
			}
		}

		if (word != m_words.end()) {
			foundAllWords = false;
		}
		else if (insertedWord != insertedWords.end()) {
			exactMatch = false;
			points += std::max(minPointsUnrecognizedWords, pointsUnrecognizedWord * static_cast<int>(insertedWords.end() - insertedWord));
		}

		if (foundAllWords) {
			points += pointsFoundAllWords;
			if (exactMatch)
				points += pointsExactMatch;
		}

		return points;
	}


	tuple<int, size_t, bool, bool> CapturingSentence::scoreBefore(const vector<string>& insertedWords) const {
		if (m_wordsAfter.empty())
			return {0, insertedWords.size(), true, true};

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
					points += std::max(minPointsUnrecognizedWords, pointsUnrecognizedWord * static_cast<int>(foundWord - insertedWord));
				}

				insertedWord = foundWord + 1;
			}
		}

		if (word != m_wordsAfter.rend()) {
			foundAllWords = false;
		}
		if (insertedWord == insertedWords.rend()) {
			points += pointsNoWordsCaptured;
		}

		return {points, insertedWords.rend() - insertedWord, foundAllWords, exactMatch};
	}
	tuple<int, size_t, bool, bool> CapturingSentence::scoreAfter(const vector<string>& insertedWords) const {
		if (m_wordsBefore.empty())
			return {0, 0, true, true};

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
					points += std::max(minPointsUnrecognizedWords, pointsUnrecognizedWord * static_cast<int>(foundWord - insertedWord));
				}

				insertedWord = foundWord + 1;
			}
		}

		if (word != m_wordsBefore.end()) {
			foundAllWords = false;
		}
		if (insertedWord == insertedWords.end()) {
			points += pointsNoWordsCaptured;
			return {points, {}, foundAllWords, exactMatch};
		}

		return {points, insertedWord - insertedWords.begin(), foundAllWords, exactMatch};
	}

	CapturingSentence::CapturingSentence(const string& sectionId, const string& sentenceId, const vector<string>& wordsBefore, const vector<string> wordsAfter, const string& code) :
		m_sectionId{sectionId}, m_sentenceId{sentenceId},
		m_wordsBefore{wordsBefore}, m_wordsAfter{wordsAfter},
		m_code{code} {}

	tuple<int, size_t, size_t> CapturingSentence::score(const vector<string>& insertedWords) const {
		int points = pointsAtBeginning;

		const auto& [pointsAfter, capturedWordsBegin, foundAllWordsAfter, exactMatchAfter] = scoreAfter(insertedWords);
		points += pointsAfter;
		const auto& [pointsBefore, capturedWordsEnd, foundAllWordsBefore, exactMatchBefore] = scoreBefore(vector<string>{insertedWords.begin() + capturedWordsBegin, insertedWords.end()});
		points += pointsBefore;

		if (foundAllWordsAfter && foundAllWordsBefore) {
			points += pointsFoundAllWords;
			if (exactMatchAfter && exactMatchBefore)
				points += pointsExactMatch;
		}

		return {points, capturedWordsBegin, capturedWordsBegin + capturedWordsEnd};
	}
}