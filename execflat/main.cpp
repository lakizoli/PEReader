// runflat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <flatbinary.hpp>
#include "executor.hpp"

static int PrintUsage (const char* exe) {
	std::cout <<
		"Usage:" << std::endl <<
		"    " << std::experimental::filesystem::path (exe).filename ().string () << " <command> <flat path>" << std::endl <<
		std::endl <<
		"  commands:" << std::endl <<
		"    run  - run the executable" << std::endl <<
		std::endl;

	return 1;
}

struct Callbacks {
};

int main (int argc, char* argv[]) {
	if (argc < 3) {
		return PrintUsage (argc > 0 ? argv[0] : "execflat");
	}

	//Load the flat binary
	std::shared_ptr<FlatBinary> binary = FlatBinary::Load (argv[2]);
	if (binary == nullptr) {
		std::cout << "Error: cannot load flat binary from file at path: " << argv[1] << std::endl << std::endl;
		return 1;
	}

	//TODO: connect (pipe) exe parameters, and exe environment to the virtual environment
	//TODO: run dll exported function

	//Execute command
	std::string command (argv[1]);
	if (command == "run") { //Run executable
		Executor executor;
		executor.RunBinary (binary);
	} else { //Unknown/unhandled command
		return PrintUsage (argv[0]);
	}

	return 0;
}
