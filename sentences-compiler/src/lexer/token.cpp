#include "token.h"

namespace lexer {
	std::string toLowercaseIfLetters(std::string value, bool isLetters) {
		return value;
	}

	Token::Token() :
		Token{static_cast<Type>(0), "", 0, 0} {}
	Token::Token(Type type, const std::string& value, unsigned int line, unsigned int column) :
		m_type{type}, m_value{value},
		m_line{line}, m_column{column} {		
		if (type == letters) {
			for (auto&& ch : m_value)
				ch = tolower(ch);
		}
	}
	
	std::string Token::str() const {
		return m_value;
	}

	bool Token::empty() const {
		return m_type == 0;
	}
	bool Token::operator==(Type type) const {
		return m_type == type;
	}
	bool Token::operator==(char ch) const {
		return m_type == Type::grammar && m_value.at(0) == ch;
	}
	bool Token::operator!=(Type type) const {
		return m_type != type;
	}
	bool Token::operator!=(char ch) const {
		return m_type != Type::grammar || m_value.at(0) != ch;
	}
	bool Token::operator&(Type type) const {
		return m_type & type;
	}

	std::string Token::position() const {
		return (m_line == 0) ? (" end of file") : (std::to_string(m_line) + ":" + std::to_string(m_column));
	}
}