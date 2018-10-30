#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_SECTION_H_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_SECTION_H_

#include <vector>
#include <string>
#include <ostream>

#include "code.h"
#include "sentence.h"

namespace parser::constructs {
	class Section {
		std::vector<Sentence> m_sentences;
		mutable std::vector<std::vector<std::string>> m_unfoldedSentences;
		Code m_code;
	public:
		Section(const std::vector<Sentence>& sentences, const Code& code);
		std::vector<std::vector<std::string>> unfoldedSentences() const;

		std::string cppSentencesList(const std::string& codeObjectName);
		std::string cppCodeStringLiteral();

		friend std::ostream& operator<< (std::ostream& stream, const Section& section);
	};

	class CapturingSection {
		std::vector<CapturingSentence> m_sentences;
		mutable std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> m_unfoldedSentences;
		Code m_code;
	public:
		CapturingSection(const std::vector<CapturingSentence>& sentences, const Code& code);
		std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> unfoldedSentences() const;

		friend std::ostream& operator<< (std::ostream& stream, const CapturingSection& section);
	};
}

#endif