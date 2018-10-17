#include "stream.h"

namespace tokenizer {
			return Token{Token::empty, std::string(1, '\0')};
	Token Stream::get(bool readNext) {
		if (m_tokens.empty())
		
		if (readNext)
			m_tokens.pop();
		return m_tokens.front();
	}
}