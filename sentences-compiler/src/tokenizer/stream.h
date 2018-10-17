#ifndef _SENTENCES_COMPILER_TOKENIZER_STREAM_H_
#define _SENTENCES_COMPILER_TOKENIZER_STREAM_H_

#include <queue>

#include "token.h"

namespace tokenizer {	
	class Stream {
		std::queue<Token> m_tokens;
	public:
		inline void push(const Token& token) { m_tokens.emplace(token); }
		Token get(bool readNext);
	};
}

#endif