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
			{"input", "the input file(s) for compilation, separated by ';' (required)", {"-i=", "--input="}, {}},
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
			std::vector<std::istream*> inputs;

			std::string filenames = args.getText("input");
			auto begin = filenames.begin();
			while (1) {
				auto end = std::find(begin, filenames.end(), ';');
				std::ifstream* file = new std::ifstream{filenames.substr(begin - filenames.begin(), end - begin), std::ios::binary};
				if (!file->is_open())
					throw std::runtime_error{"No such input file: " + filenames.substr(begin - filenames.begin(), end - begin)};
				inputs.push_back(file);

				if (end == filenames.end())
					break;					
				begin = end + 1;
			}

			std::ofstream outputFile{args.getText("output"), std::ios::binary};
			if (!outputFile.is_open())
				throw std::runtime_error{"No such output file: " + args.getText("output")};

			parser::Compiler compiler{inputs};
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