#include "sentence.h"

namespace parser::constructs {
	using std::vector;
	using std::string;
	using std::pair;


	Sentence::Sentence(const vector<OrWord>& orWords) :
		m_orWords{orWords} {}
	vector<vector<string>> Sentence::unfolded() const {
		return parser::constructs::unfolded(m_orWords.begin(), m_orWords.end());
	}

	std::ostream& operator<< (std::ostream& stream, const Sentence& sentence) {
		stream << " -";
		for (auto&& orWord : sentence.m_orWords)
			stream << " " << orWord;
		return stream;
	}


	CapturingSentence::CapturingSentence(const vector<OrWord>& orWordsBefore, const vector<OrWord>& orWordsAfter) :
		m_orWordsBefore{orWordsBefore}, m_orWordsAfter{orWordsAfter} {}
	vector<pair<vector<string>, vector<string>>> CapturingSentence::unfolded() const {
		auto unfoldedSentencesBefore = parser::constructs::unfolded(m_orWordsBefore.begin(), m_orWordsBefore.end());
		auto unfoldedSentencesAfter = parser::constructs::unfolded(m_orWordsAfter.begin(), m_orWordsAfter.end());

		vector<pair<vector<string>, vector<string>>> sentences;
		sentences.reserve(unfoldedSentencesBefore.size() * unfoldedSentencesAfter.size());

		for (auto&& sentenceBefore : unfoldedSentencesBefore)
			for (auto&& sentenceAfter : unfoldedSentencesAfter)
				sentences.emplace_back(sentenceBefore, sentenceAfter);
		
		return sentences;
	}

	std::ostream& operator<< (std::ostream& stream, const CapturingSentence& sentence) {
		stream << " *";
		for (auto&& orWord : sentence.m_orWordsBefore)
			stream << " " << orWord;
		stream << " ...";
		for (auto&& orWord : sentence.m_orWordsAfter)
			stream << " " << orWord;
		return stream;
	}
}