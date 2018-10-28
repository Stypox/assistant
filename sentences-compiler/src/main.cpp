#include <fstream>
#include <iostream>

#include "parser/parser.h"

int main() {
	try {
		std::ifstream file{"test.txt", std::ios::binary};
		parser::parse(file);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what();
		exit(1);
	}
	return 0;
}