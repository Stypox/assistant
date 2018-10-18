#include "constructs.h"

namespace parser {
	Section::Section(const std::vector<Sentence>& sentences, const Code& code) :
		m_sentences{sentences}, m_code{code} {}
	Sentence::Sentence(const std::vector<OrWord>& orWords) :
		m_orWords{orWords} {}
	OrWord::OrWord(const std::vector<std::string>& words, bool required) :
		m_words{words}, m_required{required} {}
	Code::Code(const std::string& lines) :
		m_lines{lines} {}


	std::ostream& operator<< (std::ostream& stream, const Section& section) {
		stream << "SECTION:";
		for (auto&& sentence : section.m_sentences)
			stream << "\n" << sentence;
		stream << "\n" << section.m_code;
		return stream;
	}	
	std::ostream& operator<< (std::ostream& stream, const Sentence& sentence) {
		stream << "SENTENCE:";
		for (auto&& orWord : sentence.m_orWords)
			stream << " " << orWord;
		return stream;
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
	std::ostream& operator<< (std::ostream& stream, const Code& code) {
		stream << "CODE:\n" << code.m_lines;
		return stream;
	}
}