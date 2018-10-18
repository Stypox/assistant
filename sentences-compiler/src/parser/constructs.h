#ifndef _SENTENCES_COMPILER_PARSER_SECTION_H_
#define _SENTENCES_COMPILER_PARSER_SECTION_H_

#include <vector>
#include <string>
#include <ostream>

namespace parser {
	class Code {
		std::string m_lines;
	public:
		Code(const std::string& lines);
		friend std::ostream& operator<< (std::ostream& stream, const Code& code);
	};

	class OrWord {
		std::vector<std::string> m_words;
		bool m_required;
	public:
		OrWord(const std::vector<std::string>& words, bool required);
		friend std::ostream& operator<< (std::ostream& stream, const OrWord& orWord);
	};

	class Sentence {
		std::vector<OrWord> m_orWords;
	public:
		Sentence(const std::vector<OrWord>& orWords);
		friend std::ostream& operator<< (std::ostream& stream, const Sentence& sentence);
	};

	class Section {
		std::vector<Sentence> m_sentences;
		Code m_code;
	public:
		Section(const std::vector<Sentence>& sentences, const Code& code);
		friend std::ostream& operator<< (std::ostream& stream, const Section& section);
	};
}

#endif