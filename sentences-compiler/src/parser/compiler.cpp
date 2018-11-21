#include "compiler.h"

#include "parser.h"
#include "../app/application.h"

namespace parser {
	using std::string;
	using std::to_string;

	Compiler::Compiler(const std::vector<std::istream*>& inputs) {
		const auto& [sections, capturingSections, idWhenInvalid, codeWhenInvalid] = parse(inputs);
		m_sections = std::move(sections);
		m_capturingSections = std::move(capturingSections);
		m_idWhenInvalid = std::move(idWhenInvalid);
		m_codeWhenInvalid = std::move(codeWhenInvalid);
	}

	void Compiler::toCpp(std::ostream& output) {
		constexpr std::string_view
			sectionIdObjectName =		"id_of_section_",
			idCapturingObjectName =		"id_of_capturing_section_",
			sectionIdWhenInvalidName =	"id_when_invalid_section",
			codeObjectName =			"code_of_section_",
			codeCapturingObjectName =	"code_of_capturing_section_",
			codeWhenInvalidName =		"code_when_invalid",
			parserObjectName =			"parser";

		bool pretty = app::Application::args.getBool("pretty");

		if (pretty) {
			output << "namespace sentences_compiler_gen {\n\n";
			for (size_t sectionPos = 0; sectionPos != m_sections.size(); ++sectionPos)
				output << "std::string " << sectionIdObjectName << sectionPos + 1 << "{" << m_sections[sectionPos].cppIdStringLiteral() << "};\n"
					<< "std::string " << codeObjectName << sectionPos + 1 << "{" << m_sections[sectionPos].cppCodeStringLiteral() << "};\n\n\n";
			for (size_t sectionPos = 0; sectionPos != m_capturingSections.size(); ++sectionPos)
				output << "std::string " << idCapturingObjectName << sectionPos + 1 << "{" << m_capturingSections[sectionPos].cppIdStringLiteral() << "};\n"
					<< "std::string " << codeCapturingObjectName << sectionPos + 1 << "{" << m_capturingSections[sectionPos].cppCodeStringLiteral() << "};\n\n\n";
			
			output << "std::string " << sectionIdWhenInvalidName << "{" << m_idWhenInvalid.cppStringLiteral() << "};\n";
			output << "std::string " << codeWhenInvalidName << "{" << m_codeWhenInvalid.cppStringLiteral() << "};\n\n\n";

			output << "parser::Parser " << parserObjectName << "{\n\t{";

			if (m_sections.size() != 0) {
				output << "\n\t\t// section 1" <<
					m_sections[0].cppSentencesList(string{sectionIdObjectName} + "1", string{codeObjectName} + "1", pretty);
				for (size_t sectionPos = 1; sectionPos != m_sections.size(); ++sectionPos)
					output << ",\n\n\t\t// section " << to_string(sectionPos + 1) <<
						m_sections[sectionPos].cppSentencesList(string{sectionIdObjectName} + to_string(sectionPos + 1), string{codeObjectName} + to_string(sectionPos + 1), pretty);
			}

			output << "\n\t}, {";
			if (m_capturingSections.size() != 0) {
				output << "\n\t\t// capturing section 1" <<
					m_capturingSections[0].cppSentencesList(string{idCapturingObjectName} + "1", string{codeCapturingObjectName} + "1", pretty);
				for (size_t sectionPos = 1; sectionPos != m_capturingSections.size(); ++sectionPos)
					output << ",\n\n\t\t// capturing section " << to_string(sectionPos + 1) <<
						m_capturingSections[sectionPos].cppSentencesList(string{idCapturingObjectName} + to_string(sectionPos + 1), string{codeCapturingObjectName} + to_string(sectionPos + 1), pretty);
			}

			output << "\n\t},\n\t" << sectionIdWhenInvalidName << ",\n\t" << codeWhenInvalidName << "\n};\n\n}";
		}
		else {
			output << "namespace sentences_compiler_gen {\n";
			for (size_t sectionPos = 0; sectionPos != m_sections.size(); ++sectionPos)
				output << "std::string " << sectionIdObjectName << sectionPos + 1 << "{" << m_sections[sectionPos].cppIdStringLiteral() << "};\n"
					<< "std::string " << codeObjectName << sectionPos + 1 << "{" << m_sections[sectionPos].cppCodeStringLiteral() << "};\n";
			for (size_t sectionPos = 0; sectionPos != m_capturingSections.size(); ++sectionPos)
				output << "std::string " << idCapturingObjectName << sectionPos + 1 << "{" << m_capturingSections[sectionPos].cppIdStringLiteral() << "};\n"
					<< "std::string " << codeCapturingObjectName << sectionPos + 1 << "{" << m_capturingSections[sectionPos].cppCodeStringLiteral() << "};\n";
			
			output << "std::string " << sectionIdWhenInvalidName << "{" << m_idWhenInvalid.cppStringLiteral() << "};\n";
			output << "std::string " << codeWhenInvalidName << "{" << m_codeWhenInvalid.cppStringLiteral() << "};\n";

			output << "parser::Parser " << parserObjectName << "{{";

			if (m_sections.size() != 0) {
				output << m_sections[0].cppSentencesList(string{sectionIdObjectName} + "1", string{codeObjectName} + "1", pretty);
				for (size_t sectionPos = 1; sectionPos != m_sections.size(); ++sectionPos)
					output << ",\n" << m_sections[sectionPos].cppSentencesList(string{sectionIdObjectName} + to_string(sectionPos + 1), string{codeObjectName} + to_string(sectionPos + 1), pretty);
			}

			output << "},\n{";
			if (m_capturingSections.size() != 0) {
				output << m_capturingSections[0].cppSentencesList(string{idCapturingObjectName} + "1", string{codeCapturingObjectName} + "1", pretty);
				for (size_t sectionPos = 1; sectionPos != m_capturingSections.size(); ++sectionPos)
					output << ",\n" <<
						m_capturingSections[sectionPos].cppSentencesList(string{idCapturingObjectName} + to_string(sectionPos + 1), string{codeCapturingObjectName} + to_string(sectionPos + 1), pretty);
			}

			output << "},\n" << sectionIdWhenInvalidName << ",\n" << codeWhenInvalidName << "};\n}";
		}
	}
}