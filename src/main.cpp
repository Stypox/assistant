#include <iostream>
#include <string>
#include <sstream>

#include "parser/parser.h"
#include "exec/executer.h"

#include "../sentences-compiler/sentences.cpp"

int main() {
	exec::initialize();

	while (1) {
		std::cout << "-";
		std::string line;
		std::getline(std::cin, line);
		std::stringstream linestream;
		linestream << line;
		std::vector<std::string> words;
		while (linestream >> line)
			words.push_back(line);

		if (exec::checkSignals())
			break;
		sentences_compiler_gen::parser.parse(words);
		std::cout << "\n";
	}

	return exec::finalize();
}