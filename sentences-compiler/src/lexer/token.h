#ifndef _SENTENCES_COMPILER_TOKENIZER_TOKEN_H_
#define _SENTENCES_COMPILER_TOKENIZER_TOKEN_H_

#include <string>

namespace lexer {	
	struct Token {
		enum Type : char {
			empty,
			letters,
			grammar,
			code,
			include,
		};

		Type type;
		std::string value;
		unsigned int line;

		Token(Type type, const std::string& value, unsigned int line);

		inline char ch() { return value[0]; }
		inline operator bool() { return type != empty; }
	};
}

#endif