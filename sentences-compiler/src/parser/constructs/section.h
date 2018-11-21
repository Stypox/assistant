#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_SECTION_H_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_SECTION_H_

#include <vector>
#include <string>
#include <ostream>

#include "id.h"
#include "code.h"
#include "sentence.h"

namespace parser::constructs {
	class Section {
		Id m_id;
		std::vector<Sentence> m_sentences;
		mutable std::vector<UnfoldedSentence> m_unfoldedSentences;
		Code m_code;
	public:
		Section(const Id& id, const std::vector<Sentence>& sentences, const Code& code);

		std::string cppSentencesList(const std::string& sectionIdObjectName, const std::string& codeObjectName, bool pretty) const;
		std::string cppIdStringLiteral() const;
		std::string cppCodeStringLiteral() const;

		friend std::ostream& operator<< (std::ostream& stream, const Section& section);
	};

	class CapturingSection {
		Id m_id;
		std::vector<CapturingSentence> m_sentences;
		mutable std::vector<UnfoldedCapturingSentence> m_unfoldedSentences;
		Code m_code;
	public:
		CapturingSection(const Id& id, const std::vector<CapturingSentence>& sentences, const Code& code);

		std::string cppSentencesList(const std::string& sectionIdObjectName, const std::string& codeObjectName, bool pretty) const;
		std::string cppIdStringLiteral() const;
		std::string cppCodeStringLiteral() const;

		friend std::ostream& operator<< (std::ostream& stream, const CapturingSection& section);
	};
}

#endif