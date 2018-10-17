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

	class ImportanceWord {
		std::string m_letters;
		bool m_required;
	public:
		ImportanceWord(const std::string& letters, bool required);
		friend std::ostream& operator<< (std::ostream& stream, const ImportanceWord& importanceWord);
	};

	class OrWord {
		std::vector<ImportanceWord> m_importanceWords;
	public:
		OrWord(const std::vector<ImportanceWord>& importanceWords);
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