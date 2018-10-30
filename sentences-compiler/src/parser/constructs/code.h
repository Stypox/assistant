#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_CODE_H_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_CODE_H_

#include <string>

namespace parser::constructs {
	class Code : public std::string {
	public:
		Code cppStringLiteral() const;
	};
}

#endif