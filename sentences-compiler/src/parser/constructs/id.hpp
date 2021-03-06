#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_ID_HPP_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_ID_HPP_

#include <string>

namespace parser::constructs {
	class Id : public std::string {
	public:
		std::string cppStringLiteral() const;
	};
}

#endif