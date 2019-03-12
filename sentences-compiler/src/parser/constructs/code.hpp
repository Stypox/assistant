#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_CODE_HPP_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_CODE_HPP_

#include <string>

namespace parser::constructs {
	class Code : public std::string {
	public:
		std::string cppStringLiteral() const;
	};
}

#endif