#include <iostream>

#include "tokenizer/tokenizer.h"

int main() {
	try {
		tokenizer::Stream ts = tokenizer::tokenize(std::cin);
		for (auto token = ts.get(); token; token = ts.get()) {
			std::cout << static_cast<int>(token.type) << " " << token.value << "\n";
		}
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what();
		exit(1);
	}
	return 0;
}