#ifndef _TOKENIZER_STREAM_H_
#define _TOKENIZER_STREAM_H_

#include <queue>

#include "token.h"

namespace tokenizer {	
	class Stream {
		std::queue<Token> m_tokens;
	public:
		inline void push(const Token& token) { m_tokens.emplace(token); }
		inline Token getKeep() { return m_tokens.front(); }
		Token get();
	};
}

#endif