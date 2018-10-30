#include "section.h"

namespace parser::constructs {
	using std::vector;
	using std::string;
	using std::pair;


	Section::Section(const vector<Sentence>& sentences, const Code& code) :
		m_sentences{sentences}, m_unfoldedSentences{},
		m_code{code} {}
	vector<vector<string>> Section::unfoldedSentences() const {
		if (m_unfoldedSentences.empty()) {
			for (auto&& sentence : m_sentences) {
				auto unfolded = sentence.unfolded();
				m_unfoldedSentences.insert(m_unfoldedSentences.end(), unfolded.begin(), unfolded.end());
			}
		}
		return m_unfoldedSentences;
	}

	std::ostream& operator<< (std::ostream& stream, const Section& section) {
		stream << "SECTION:";
		for (auto&& sentence : section.m_sentences)
			stream << "\n" << sentence;
		stream << "\nUNFOLDED:";
		for (auto&& sentence : section.unfoldedSentences()) {
			stream << "\n -";
			for (auto&& word : sentence)
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}


	CapturingSection::CapturingSection(const vector<CapturingSentence>& sentences, const Code& code) :
		m_sentences{sentences}, m_unfoldedSentences{},
		m_code{code} {}
	vector<pair<vector<string>, vector<string>>> CapturingSection::unfoldedSentences() const {
		if (m_unfoldedSentences.empty()) {
			for (auto&& sentence : m_sentences) {
				auto unfolded = sentence.unfolded();
				m_unfoldedSentences.insert(m_unfoldedSentences.end(), unfolded.begin(), unfolded.end());
			}
		}
		return m_unfoldedSentences;
	}

	std::ostream& operator<< (std::ostream& stream, const CapturingSection& section) {
		stream << "SECTION (with capturing group):";
		for (auto&& sentence : section.m_sentences)
			stream << "\n" << sentence;
		stream << "\nUNFOLDED:";
		for (auto&& sentence : section.unfoldedSentences()) {
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
}