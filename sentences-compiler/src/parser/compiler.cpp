#include "compiler.h"

#include "parser.h"
#include "../app/application.h"

namespace parser {
	Compiler::Compiler(const std::vector<std::istream*>& inputs) {
		const auto& [codeWhenNotUnderstood, sections, capturingSections] = parse(inputs);
		m_codeWhenNotUnderstood = codeWhenNotUnderstood;
		m_sections = sections;
		m_capturingSections = capturingSections;
	}

	void Compiler::toCpp(std::ostream& output) {
		bool pretty = app::Application::args.getBool("pretty");

		constexpr std::string_view codeObjectName = "code_of_section_";
		constexpr std::string_view codeCapturingObjectName = "code_of_capturing_section_";
		constexpr std::string_view parserObjectName = "parser";

		if (pretty) {
			output << "namespace sentences_compiler_gen {\n\n";
			for (size_t sectionPos = 0; sectionPos != m_sections.size(); ++sectionPos)
				output << "std::string " << codeObjectName << sectionPos + 1 << "{" << m_sections[sectionPos].cppCodeStringLiteral() << "};\n\n\n";
			for (size_t sectionPos = 0; sectionPos != m_capturingSections.size(); ++sectionPos)
				output << "std::string " << codeCapturingObjectName << sectionPos + 1 << "{" << m_capturingSections[sectionPos].cppCodeStringLiteral() << "};\n\n\n";

			output << "parser::Parser " << parserObjectName << "{\n\t{";

			if (m_sections.size() != 0) {
				output << "\n\t\t// section 1" << m_sections[0].cppSentencesList(std::string{codeObjectName} + "1", pretty);
				for (size_t sectionPos = 1; sectionPos != m_sections.size(); ++sectionPos)
					output << ",\n\n\t\t// section " << std::to_string(sectionPos + 1) << m_sections[sectionPos].cppSentencesList(std::string{codeObjectName} + std::to_string(sectionPos + 1), pretty);
			}

			output << "\n\t}, {";
			if (m_capturingSections.size() != 0) {
				output << "\n\t\t// capturing section 1" << m_capturingSections[0].cppSentencesList(std::string{codeCapturingObjectName} + "1", pretty);
				for (size_t sectionPos = 1; sectionPos != m_capturingSections.size(); ++sectionPos)
					output << ",\n\n\t\t// capturing section " << std::to_string(sectionPos + 1) << m_capturingSections[sectionPos].cppSentencesList(std::string{codeCapturingObjectName} + std::to_string(sectionPos + 1), pretty);
			}

			output << "\n\t},\n\t" << m_codeWhenNotUnderstood.cppStringLiteral() << "\n};\n\n}";
		}
		else {
			output << "namespace sentences_compiler_gen {\n";
			for (size_t sectionPos = 0; sectionPos != m_sections.size(); ++sectionPos)
				output << "std::string " << codeObjectName << sectionPos + 1 << "{" << m_sections[sectionPos].cppCodeStringLiteral() << "};\n";
			for (size_t sectionPos = 0; sectionPos != m_capturingSections.size(); ++sectionPos)
				output << "std::string " << codeCapturingObjectName << sectionPos + 1 << "{" << m_capturingSections[sectionPos].cppCodeStringLiteral() << "};\n";

			output << "parser::Parser " << parserObjectName << "{{";

			if (m_sections.size() != 0) {
				output << m_sections[0].cppSentencesList(std::string{codeObjectName} + "1", pretty);
				for (size_t sectionPos = 1; sectionPos != m_sections.size(); ++sectionPos)
					output << ",\n" << m_sections[sectionPos].cppSentencesList(std::string{codeObjectName} + std::to_string(sectionPos + 1), pretty);
			}

			output << "},\n{";
			if (m_capturingSections.size() != 0) {
				output << m_capturingSections[0].cppSentencesList(std::string{codeCapturingObjectName} + "1", pretty);
				for (size_t sectionPos = 1; sectionPos != m_capturingSections.size(); ++sectionPos)
					output << ",\n" << m_capturingSections[sectionPos].cppSentencesList(std::string{codeCapturingObjectName} + std::to_string(sectionPos + 1), pretty);
			}

			output << "},\n" << m_codeWhenNotUnderstood.cppStringLiteral() << "};\n}";
		}
	}
}