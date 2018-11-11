#include "application.h"

#include <iostream>
#include <fstream>

namespace app {
	stypox::ArgParser Application::currentArgsInitializer{
		"",
		{
			{"help", "prints the help screen and exits", {"-?", "-h", "--help"}},
			{"quit", "quit", {"-q", "--quit"}}
		}, {

		}, {

		}, {
			{"sentences", "replaces the precompiled sentences (format: defId,defCodeHex;id,word word,codeHex;id,word word ,word word,codeHex;...)", {"-s=", "--sentences="}}
		}
	};

	stypox::ArgParser Application::initialArgs{
		"assistant",
		{
			{"help", "prints the help screen and exits", {"-?", "-h", "--help"}}
		}, {

		}, {

		}, {
			{"output", "the output file (defaults to stdout)", {"-o=", "--output="}, "stdout"},
			{"encoding", "input encoding (values: 8bit (default), 2x8bit (= 16bit split in two 8bit))", {"-e=", "--encoding="}, "8bit", [](std::string s) { return s == "8bit" || s == "2x8bit"; }},
			{"format", "output format (values: json (default))", {"-f=", "--format="}, "json", [](std::string s) { return s == "json"; }},
			{"logs", "the file to save logs in (when openable) (stdout is valid)", {"-l=", "--logs="}}
		}
	};
	std::unique_ptr<std::ostream> Application::logs{};


	bool Application::parseInitialArgs(int argc, char const *argv[]) {
		try {
			initialArgs.parse(argc, argv);
			
			if (std::string logsFilename = initialArgs.getText("logs"); logsFilename == "stdout") {
				logs.reset(&std::cout);
			}
			else if (logsFilename != "") {
				auto file = new std::ofstream{logsFilename, std::ios::binary};
				if (file->is_open())
					logs.reset(file);
			}

			if (initialArgs.getBool("help")) {
				std::cout << initialArgs.help();
				return false;
			}
			
			initialArgs.validate();
		}
		catch (const std::runtime_error& e) {
			if (logs.get() != nullptr)
				*logs << "Error while parsing arguments: " << e.what();
			return false;
		}
		
		if (std::string outputFilename = initialArgs.getText("output"); outputFilename == "stdout") {
			output.reset(&std::cout);
		}
		else {
			auto file = new std::ofstream{outputFilename, std::ios::binary};
			if (file->is_open())
				output.reset(file);
			else
				return false;
		}

		return true;
	}


	int Application::run(int argc, char const *argv[]) {
		if (!parseInitialArgs(argc, argv))
			return !initialArgs.getBool("help");
		
		
	}
}