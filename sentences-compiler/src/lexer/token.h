#ifndef _SENTENCES_COMPILER_TOKENIZER_TOKEN_H_
#define _SENTENCES_COMPILER_TOKENIZER_TOKEN_H_

#include <string>

namespace lexer {	
	class Token {
	public:
		enum Type : char {
			letters = 1,
			grammar = 2,
			code = 3,
			include = 4,
		};

	private:
		Type m_type;
		std::string m_value;
		unsigned int m_line, m_column;
	
	public:
		Token();
		Token(Type type, const std::string& value, unsigned int line, unsigned int column);

		std::string str() const;

		bool empty() const;
		bool operator==(Type type) const;
		bool operator==(char ch) const;
		bool operator!=(Type type) const;
		bool operator!=(char ch) const;

		std::string position() const;
	};
}

#endif