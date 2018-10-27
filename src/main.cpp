#include <iostream>
#include "parser/parser.h"

int main() {
	constexpr parser::ConstSentence<4> s{"a", {"b", "c", "d"}};
	parser::Sentence s1 {"a", {"b", "c", "d"}};
	parser::Parser<std::array<parser::Sentence, 1>, std::array<parser::ConstSentence<4>, 1>, std::vector<parser::Sentence>> a = make_parser(s1, s, std::vector<parser::Sentence>{s1});
	return 0;
}
