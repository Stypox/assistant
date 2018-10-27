#include <iostream>
#include "parser/parser.h"

int main() {
	parser::Parser p{{{{"Hello"}, "print(\"Ok\")"},
					  {{"Hey"}, "print(\"Ok\")"}},
					  "print(\"Bad\")"};
	p.parse({"Hey"});
	return 0;
}