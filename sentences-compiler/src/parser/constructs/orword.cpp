#include "orword.hpp"

namespace parser::constructs {
	using std::vector;
	using std::string;


	OrWord::OrWord(const vector<string>& words, bool required) :
		m_words{words}, m_required{required} {}

	vector<vector<string>> unfolded(vector<OrWord>::const_iterator thisWord, vector<OrWord>::const_iterator lastWord) {
		if (thisWord >= lastWord)
			return {vector<string>{}};
		
		auto nextWordsUnfolded = unfolded(thisWord + 1, lastWord);

		vector<vector<string>> sentences = ((thisWord->m_required) ? (vector<vector<string>>{}) : (nextWordsUnfolded));
		sentences.reserve(sentences.size() + nextWordsUnfolded.size() * thisWord->m_words.size());

		for (auto&& unfoldedSentence : nextWordsUnfolded) {
			vector<string> sentence {string{""}};
			sentence.insert(sentence.end(), unfoldedSentence.begin(), unfoldedSentence.end());
			for (auto&& unfoldedWord : thisWord->m_words) {
				sentence[0] = unfoldedWord;
				sentences.push_back(sentence);
			}
		}

		return sentences;
	}
	std::ostream& operator<< (std::ostream& stream, const OrWord& orWord) {
		if (!orWord.m_required)
			stream << "[";
		stream << orWord.m_words[0];
		for (auto it = orWord.m_words.begin() + 1; it < orWord.m_words.end(); ++it)
			stream << "/" << *it;
		if (!orWord.m_required)
			stream << "]";
		return stream;
	}
}