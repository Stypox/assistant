#ifndef _TOKENIZER_TOKENIZER_H_
#define _TOKENIZER_TOKENIZER_H_

#include <istream>

#include "stream.h"

namespace tokenizer {
	Stream tokenize(std::istream& input);
}

#endif