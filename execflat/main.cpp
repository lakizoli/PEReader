// runflat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <flatbinary.hpp>
#include <x86cpu.h>
//#include <bochs/disasm/disasm.h>
//#include "cpuwrapper.hpp"

static int PrintUsage (const char* exe) {
	std::cout <<
		"Usage:" << std::endl <<
		"    " << std::experimental::filesystem::path (exe).filename ().string () << " <command> <flat path>" << std::endl <<
		std::endl <<
		"  commands:" << std::endl <<
		"    analyze - analyze the executable" << std::endl <<
		"    run  - run the executable" << std::endl <<
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

	//Execute command
	std::string command (argv[1]);
	if (command == "analyze") { //Analyze executable
		//TODO: ...

		////Dump disassembly
		//const std::vector<uint8_t>& data = binary->GetBinary ();
		//uint64_t entryPoint = binary->GetEntryPoint ();
		//uint64_t virtualBase = binary->GetVirtualBase ();
		//disassembler dis;

		//char commandBuffer[1024];
		//const uint8_t* ip = &data[entryPoint - virtualBase];
		//uint32_t res = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
		//while (true) {
		//	ip += res;
		//	res = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
		//}
	} else if (command == "run") { //Run executable
		std::unique_ptr<bx_pc_system_c> sys (new bx_pc_system_c ());
		std::unique_ptr<BX_MEM_C> mem (new BX_MEM_C ());
		std::unique_ptr<BX_CPU_C> cpu (new BX_CPU_C (mem.get (), sys.get ()));

		//Cpu cpu;
		//cpu.InitStack (0x200000, binary->GetStackSize ());
		//cpu.SetFlatBinary (binary->GetVirtualBase (), binary->GetBinary ());

		//cpu.Call (binary->GetEntryPoint ());
	}

	return 0;
}
