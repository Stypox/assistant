#ifndef _SENTENCES_COMPILER_PARSER_COMPILER_H_
#define _SENTENCES_COMPILER_PARSER_COMPILER_H_

#include "constructs/section.hpp"

namespace parser {
	class Compiler {
		std::vector<constructs::Section> m_sections;
		std::vector<constructs::CapturingSection> m_capturingSections;
		constructs::Id m_idWhenInvalid;
		constructs::Code m_codeWhenInvalid;
	public:
		Compiler(const std::vector<std::istream*>& inputs);

		void toCpp(std::ostream& output);
	};
}

#endif