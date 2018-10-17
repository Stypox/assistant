#include "constructs.h"

namespace parser {
	Section::Section(const std::vector<Sentence>& sentences, const Code& code) :
		m_sentences{sentences}, m_code{code} {}
	Sentence::Sentence(const std::vector<OrWord>& orWords) :
		m_orWords{orWords} {}
	OrWord::OrWord(const std::vector<ImportanceWord>& importanceWords) :
		m_importanceWords{importanceWords} {}
	ImportanceWord::ImportanceWord(const std::string& letters, bool required) :
		m_letters{letters}, m_required{required} {}
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
		stream << orWord.m_importanceWords[0];
		for (auto it = orWord.m_importanceWords.begin() + 1; it < orWord.m_importanceWords.end(); ++it)
			stream << "/" << *it;
		return stream;
	}
	std::ostream& operator<< (std::ostream& stream, const ImportanceWord& importanceWord) {
		if (importanceWord.m_required)
			stream << importanceWord.m_letters;
		else
			stream << "[" << importanceWord.m_letters << "]";
		return stream;
	}
	std::ostream& operator<< (std::ostream& stream, const Code& code) {
		stream << "CODE:\n" << code.m_lines;
		return stream;
	}
}