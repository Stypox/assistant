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
		
	}
}