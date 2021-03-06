// runflat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <flatbinary.hpp>
#include "analyzer.hpp"
//#include "cpuwrapper.hpp"

static int PrintUsage (const char* exe) {
	std::cout <<
		"Usage:" << std::endl <<
		"    " << std::experimental::filesystem::path (exe).filename ().string () << " <command> <flat path> [destination path]" << std::endl <<
		std::endl <<
		"  commands:" << std::endl <<
		"    analyze - analyze the executable, and save result to the destination path" << std::endl <<
		//"    run  - run the executable" << std::endl <<
		std::endl;

	return 1;
}

int main (int argc, char* argv[]) {
	if (argc < 3) {
		return PrintUsage (argc > 0 ? argv[0] : "runflat");
	}

	//Load the flat binary
	std::shared_ptr<FlatBinary> binary = FlatBinary::Load (argv[2]);
	if (binary == nullptr) {
		std::cout << "Error: cannot load flat binary from file at path: " << argv[1] << std::endl << std::endl;
		return 1;
	}

	//TODO: rename runflat project to analyzer

	//Execute command
	std::string command (argv[1]);
	if (command == "analyze") { //Analyze executable
		if (argc < 4) {
			return PrintUsage (argv[0]);
		}

		Analyzer analyzer (argv[2], argv[3]);
		analyzer.Execute (binary);
		analyzer.SaveAssemblyFiles ();
		analyzer.SaveUsedCommandsFile ();


	//} else if (command == "run") { //Run executable
	//	Cpu cpu;
	//	cpu.InitStack (0x200000, binary->GetStackSize ());
	//	cpu.SetFlatBinary (binary->GetVirtualBase (), binary->GetBinary ());

	//	cpu.Call (binary->GetEntryPoint ());
	} else { //Unknown/unhandled command
		return PrintUsage (argv[0]);
	}

    return 0;
}
