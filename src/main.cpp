#include <iostream>
#include "parser/parser.h"

int main() {
	parser::Parser p{{{{"Hello", "how", "are", "you"}, "print(\"Ok\")"},
					  {{"Hey", "Hello"}, "print(\"Ok\")"}},
					  "print(\"Bad\")"};
	p.parse({"Hello"});
	return 0;
}