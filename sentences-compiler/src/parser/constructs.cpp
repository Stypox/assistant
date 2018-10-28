#include "constructs.h"

namespace parser {
	using std::vector;
	using std::string;
	using std::pair;


	vector<vector<string>> unfold(vector<OrWord>::iterator thisWord, vector<OrWord>::iterator lastWord) {
		if (thisWord >= lastWord)
			return {vector<string>{}};
		
		auto nextWordsUnfolded = unfold(thisWord + 1, lastWord);

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


	Section::Section(const vector<Sentence>& sentences, const Code& code) :
		m_sentences{sentences}, m_code{code} {}
	void Section::unfoldSentences() {
		if (!m_unfoldedSentences.empty())
			return;

		for (auto&& sentence : m_sentences) {
			auto unfoldedSentences = sentence.unfold();
			m_unfoldedSentences.insert(m_unfoldedSentences.end(), unfoldedSentences.begin(), unfoldedSentences.end());
		}
	}
	std::ostream& operator<< (std::ostream& stream, const Section& section) {
		stream << "SECTION:";
		for (auto&& sentence : section.m_sentences)
			stream << "\n" << sentence;
		stream << "\nUNFOLDED:";
		for (auto&& sentence : section.m_unfoldedSentences) {
			stream << "\n -";
			for (auto&& word : sentence)
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}


	CapturingSection::CapturingSection(const vector<CapturingSentence>& sentences, const Code& code) :
		m_sentences{sentences}, m_code{code} {}
	void CapturingSection::unfoldSentences() {
		if (!m_unfoldedSentences.empty())
			return;

		for (auto&& sentence : m_sentences) {
			auto unfoldedSentences = sentence.unfold();
			m_unfoldedSentences.insert(m_unfoldedSentences.end(), unfoldedSentences.begin(), unfoldedSentences.end());
		}
	}
	std::ostream& operator<< (std::ostream& stream, const CapturingSection& section) {
		stream << "SECTION (with capturing group):";
		for (auto&& sentence : section.m_sentences)
			stream << "\n" << sentence;
		stream << "\nUNFOLDED:";
		for (auto&& sentence : section.m_unfoldedSentences) {
			stream << "\n *";
			for (auto&& word : sentence.first)
				stream << " " << word;
			stream << " ...";
			for (auto&& word : sentence.second)
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}


	Sentence::Sentence(const vector<OrWord>& orWords) :
		m_orWords{orWords} {}
	vector<vector<string>> Sentence::unfold() {
		return parser::unfold(m_orWords.begin(), m_orWords.end());
	}
	std::ostream& operator<< (std::ostream& stream, const Sentence& sentence) {
		stream << " -";
		for (auto&& orWord : sentence.m_orWords)
			stream << " " << orWord;
		return stream;
	}


	CapturingSentence::CapturingSentence(const vector<OrWord>& orWordsBefore, const vector<OrWord>& orWordsAfter) :
		m_orWordsBefore{orWordsBefore}, m_orWordsAfter{orWordsAfter} {}
	vector<pair<vector<string>, vector<string>>> CapturingSentence::unfold() {
		auto unfoldedSentencesBefore = parser::unfold(m_orWordsBefore.begin(), m_orWordsBefore.end());
		auto unfoldedSentencesAfter = parser::unfold(m_orWordsAfter.begin(), m_orWordsAfter.end());

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


	OrWord::OrWord(const vector<string>& words, bool required) :
		m_words{words}, m_required{required} {}
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