#include "token.h"

namespace tokenizer {
	Token::Token(Type type, std::string value) :
		type{type}, value{value} {
		if (type == word) {
			for (auto&& ch : value)
				ch = tolower(ch);
		}
	}
}