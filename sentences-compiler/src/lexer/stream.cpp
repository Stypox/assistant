#include "stream.hpp"

namespace lexer {
	Token Stream::get(bool readNext) {
		if (m_tokens.empty())
			return {};
		
		if (readNext)
			m_tokens.pop();
		return m_tokens.front();
	}
}