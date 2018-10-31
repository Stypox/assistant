#include "compiler.h"

#include "parser.h"

namespace parser {
	Compiler::Compiler(std::istream& input) {
		const auto& [codeWhenNotUnderstood, sections, capturingSections] = parse(input);
		m_codeWhenNotUnderstood = codeWhenNotUnderstood;
		m_sections = sections;
		m_capturingSections = capturingSections;
	}

	void Compiler::toCpp(std::ostream& output) {
		constexpr std::string_view codeObjectName = "code_of_section_";
		constexpr std::string_view codeCapturingObjectName = "code_of_capturing_section_";
		constexpr std::string_view parserObjectName = "parser";

		output << "namespace sentences_compiler_gen {\n";

		for (size_t sectionPos = 0; sectionPos != m_sections.size(); ++sectionPos)
			output << "std::string " << codeObjectName << sectionPos + 1 << "{" << m_sections[sectionPos].cppCodeStringLiteral() << "};\n";
		for (size_t sectionPos = 0; sectionPos != m_capturingSections.size(); ++sectionPos)
			output << "std::string " << codeCapturingObjectName << sectionPos + 1 << "{" << m_capturingSections[sectionPos].cppCodeStringLiteral() << "};\n";

		output << "parser::Parser " << parserObjectName << "{{";
		if (m_sections.size() != 0)
			output << m_sections[0].cppSentencesList(std::string{codeObjectName} + "1");
		for (size_t sectionPos = 1; sectionPos != m_sections.size(); ++sectionPos)
			output << ",\n" << m_sections[sectionPos].cppSentencesList(std::string{codeObjectName} + std::to_string(sectionPos + 1));

		output << "},\n{";
		if (m_capturingSections.size() != 0)
			output << m_capturingSections[0].cppSentencesList(std::string{codeCapturingObjectName} + "1");
		for (size_t sectionPos = 1; sectionPos != m_capturingSections.size(); ++sectionPos)
			output << ",\n" << m_capturingSections[sectionPos].cppSentencesList(std::string{codeCapturingObjectName} + std::to_string(sectionPos + 1));

		output << "},\n" << m_codeWhenNotUnderstood.cppStringLiteral() << "};\n}";
	}
}