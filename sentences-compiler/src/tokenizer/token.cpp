#include "token.h"

namespace tokenizer {
	Token::Token(Type type, std::string value, unsigned int line) :
		type{type}, value{value}, line{line} {
		if (type == letters) {
			for (auto&& ch : value)
				ch = tolower(ch);
		}
	}
}