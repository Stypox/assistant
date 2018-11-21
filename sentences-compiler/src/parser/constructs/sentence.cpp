#include "sentence.h"

namespace parser::constructs {
	using std::vector;
	using std::string;
	using std::pair;


	Sentence::Sentence(const vector<OrWord>& orWords, const std::optional<Id>& id) :
		m_id{id.has_value() ? *id : Id{}}, m_orWords{orWords} {}
	vector<UnfoldedSentence> Sentence::unfolded() const {
		auto unfoldedSentence = parser::constructs::unfolded(m_orWords.begin(), m_orWords.end());

		vector<UnfoldedSentence> sentences;
		sentences.reserve(unfoldedSentence.size());

		for (auto&& sentence : unfoldedSentence)
			sentences.push_back({m_id, sentence});
		
		return sentences;
	}

	std::ostream& operator<< (std::ostream& stream, const Sentence& sentence) {
		stream << " <" << sentence.m_id << ">";
		for (auto&& orWord : sentence.m_orWords)
			stream << " " << orWord;
		return stream;
	}


	CapturingSentence::CapturingSentence(const vector<OrWord>& orWordsBefore, const vector<OrWord>& orWordsAfter, const std::optional<Id>& id) :
		m_id{id.has_value() ? *id : Id{}}, m_orWordsBefore{orWordsBefore},
		m_orWordsAfter{orWordsAfter} {}
	vector<UnfoldedCapturingSentence> CapturingSentence::unfolded() const {
		auto unfoldedSentenceBefore = parser::constructs::unfolded(m_orWordsBefore.begin(), m_orWordsBefore.end());
		auto unfoldedSentenceAfter = parser::constructs::unfolded(m_orWordsAfter.begin(), m_orWordsAfter.end());

		vector<UnfoldedCapturingSentence> sentences;
		sentences.reserve(unfoldedSentenceBefore.size() * unfoldedSentenceAfter.size());

		for (auto&& sentenceBefore : unfoldedSentenceBefore)
			for (auto&& sentenceAfter : unfoldedSentenceAfter)
				sentences.push_back({m_id, sentenceBefore, sentenceAfter});
		
		return sentences;
	}

	std::ostream& operator<< (std::ostream& stream, const CapturingSentence& sentence) {
		stream << " <" << sentence.m_id << ">";
		for (auto&& orWord : sentence.m_orWordsBefore)
			stream << " " << orWord;
		stream << " ...";
		for (auto&& orWord : sentence.m_orWordsAfter)
			stream << " " << orWord;
		return stream;
	}
}