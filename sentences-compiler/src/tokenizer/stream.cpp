#include "stream.h"

namespace tokenizer {
	Token Stream::get() {
		if (m_tokens.size() == 0)
			return Token{Token::empty, std::string(1, '\0')};
		
		Token front = m_tokens.front();
		m_tokens.pop();
		return front;
	}
}