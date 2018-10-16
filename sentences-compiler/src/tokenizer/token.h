#ifndef _SENTENCES_COMPILER_TOKENIZER_TOKEN_H_
#define _SENTENCES_COMPILER_TOKENIZER_TOKEN_H_

#include <string>

namespace tokenizer {	
	struct Token {
		enum Type : char {
			empty,
			word,
			grammar,
			code,
		};

		Type type;
		std::string value;

		Token(Type type, std::string value);

		inline char ch() { return value[0]; }
		inline operator bool() { return type != empty; }
	};
}

#endif