#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_ID_H_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_ID_H_

#include <string>

namespace parser::constructs {
	class Id : public std::string {
	public:
		std::string cppStringLiteral() const;
	};
}

#endif