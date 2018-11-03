#ifndef _SENTENCES_COMPILER_PARSER_COMPILER_H_
#define _SENTENCES_COMPILER_PARSER_COMPILER_H_

#include "constructs/section.h"

namespace parser {
	class Compiler {
		constructs::Code m_codeWhenNotUnderstood;
		std::vector<constructs::Section> m_sections;
		std::vector<constructs::CapturingSection> m_capturingSections;
	public:
		Compiler(const std::vector<std::istream*>& inputs);

		void toCpp(std::ostream& output);
	};
}

#endif