#include "application.h"

#include <iostream>
#include <fstream>

#include "../parser/compiler.h"

namespace app {
	stypox::ArgParser Application::args{
		"sentences-compiler",
		{
			{"help", "prints the help screen", {"-?", "-h", "--help"}},
			{"pretty", "makes the output code prettier and human readable", {"-p", "--pretty"}},
			{"verbose", "makes the program output more things", {"-v", "--verbose"}}
		}, {

		}, {

		}, {
			{"input", "the input file for compilation (required)", {"-i=", "--input="}, {}},
			{"output", "the output file for compilation (required)", {"-o=", "--output="}, {}},
			{"language", "the language targeted at compilation (values: c++ (default))", {"-l=", "--language="}, "c++", [](std::string s) { return s == "c++"; }},
		}
	};

	int Application::run(int argc, char const *argv[]) {
		try {
			args.parse(argc, argv);
			if (args.getBool("help")) {
				std::cout << args.help();
				return 0;
			}
			args.validate();
		}
		catch (const std::runtime_error& e) {
			std::cout << "Error while parsing arguments: " << e.what();
			return 1;
		}

		try {
			std::ifstream inputFile{args.getText("input"), std::ios::binary};
			std::ofstream outputFile{args.getText("output"), std::ios::binary};
			parser::Compiler compiler{inputFile}; // TODO accept more than one file

			if (auto lang = args.getText("language"); lang == "c++")
				compiler.toCpp(outputFile);
		}
		catch (const std::runtime_error& e) {
			std::cout << "Error while compiling: " << e.what();
			return 1;
		}

		return 0;
	}
}