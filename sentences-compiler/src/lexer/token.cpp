#include "token.h"

namespace lexer {
	Token::Token(Type p_type, const std::string& p_value, unsigned int p_line) :
		type{p_type}, value{p_value}, line{p_line} {
		if (type == letters) {
			for (auto&& ch : value)
				ch = tolower(ch);
		}
	}
}