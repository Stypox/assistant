#include <fstream>
#include <iostream>

#include "parser/compiler.h"

int main() {
	try {
		std::ifstream file{"test.txt", std::ios::binary};
		parser::Compiler compiler{file};
		compiler.toCpp(std::cout);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what();
		exit(1);
	}
	return 0;
}