#include "application.h"

#include <iostream>
#include <fstream>

#include "../parser/compiler.h"

namespace app {
	bool Application::help = false,
		Application::pretty = false,
		Application::verbose = false;
	std::string Application::input{},
		Application::output{},
		Application::language{"c++"};

	using stypox::args;
	stypox::ArgParser argParser{
		std::make_tuple(
			stypox::SwitchOption{"help", Application::help, args("-?", "-h", "--help"), "prints the help screen and exits"},
			stypox::SwitchOption{"pretty", Application::pretty, args("-p", "--pretty"), "makes the output code prettier and human readable"},
			stypox::SwitchOption{"verbose", Application::verbose, args("-v", "--verbose"), "will print (more) debug messages"},
			stypox::Option{"input", Application::input, args("-i=", "--input="), "the input file(s) for compilation, separated by ':' (required)", true},
			stypox::Option{"output", Application::output, args("-o=", "--output="), "the output file for compilation (required)", true},
			stypox::Option{"language", Application::language, args("-l=", "--language="), "the language targeted at compilation (values: c++ (default))"}
		),
		"sentences-compiler by Stypox"
	};

	int Application::run(int argc, char const *argv[]) {
		try {
			argParser.parse(argc, argv);
			if (help) {
				std::cout << argParser.help();
				return 0;
			}
			argParser.validate();
		}
		catch (const std::runtime_error& e) {
			std::cout << "Error while parsing arguments: " << e.what();
			return 1;
		}

		try {
			std::vector<std::istream*> inputs;

			auto begin = input.begin();
			while (1) {
				auto end = std::find(begin, input.end(), ':');
				std::ifstream* file = new std::ifstream{input.substr(begin - input.begin(), end - begin), std::ios::binary};
				if (!file->is_open())
					throw std::runtime_error{"No such input file: " + input.substr(begin - input.begin(), end - begin)};
				inputs.push_back(file);

				if (end == input.end())
					break;					
				begin = end + 1;
			}

			std::ofstream outputFile{output, std::ios::binary};
			if (!outputFile.is_open())
				throw std::runtime_error{"No such output file: " + output};

			parser::Compiler compiler{inputs};
			if (language == "c++")
				compiler.toCpp(outputFile);
		}
		catch (const std::runtime_error& e) {
			std::cout << "Error while compiling: " << e.what();
			return 1;
		}

		return 0;
	}
}