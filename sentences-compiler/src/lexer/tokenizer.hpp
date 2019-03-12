#ifndef _SENTENCES_COMPILER_LEXER_LEXER_HPP_
#define _SENTENCES_COMPILER_LEXER_LEXER_HPP_

#include <istream>

#include "stream.hpp"

namespace lexer {
	Stream tokenize(std::istream& input);
}

#endif