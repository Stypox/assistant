#include <iostream>

#include "parser/parser.h"

int main() {
	try {
		parser::Parser parser;
		parser.parse(std::cin);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what();
		exit(1);
	}
	return 0;
}