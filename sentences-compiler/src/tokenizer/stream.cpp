#include "stream.h"

namespace tokenizer {
	Token Stream::get(bool readNext) {
		if (m_tokens.empty())
			return Token{Token::empty, std::string(1, '\0'), 0};
		
		if (readNext)
			m_tokens.pop();
		return m_tokens.front();
	}
}