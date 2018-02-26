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

		//Init memory
		uint64_t memSize = 16 * 1024 * 1024; //Define 16 mb memory in virtual environment
		uint64_t hostMemSize = 16 * 1024 * 1024; //Allocate 16 mb memory on host computer
		mem->init_memory (memSize, hostMemSize);

		for (uint64_t i = 0; i < memSize / 0x1000; ++i) {
			uint8_t* ptr = mem->get_vector (cpu.get (), i * 0x1000);
			memset (ptr, 0, 0x1000);
		}

		//Copy program into memory
		const std::vector<uint8_t>& data = binary->GetBinary ();
		uint64_t virtualBase = binary->GetVirtualBase ();

		uint64_t dataSize = data.size ();
		uint64_t pageCount = dataSize / 0x1000;
		if (dataSize % 0x1000 > 0) {
			++pageCount;
		}

		for (uint64_t i = 0; i < pageCount; ++i) {
			uint64_t pos = i * 0x1000;
			uint64_t len = dataSize - pos > 0x1000 ? 0x1000 : dataSize - pos;

			uint8_t* ptr = mem->get_vector (cpu.get (), 0x100000 + virtualBase + pos);
			memcpy (ptr, &data[pos], len);

			pos += len;
		}

		//Init stack

		//Run program


		//Cpu cpu;
		//cpu.InitStack (0x200000, binary->GetStackSize ());
		//cpu.SetFlatBinary (binary->GetVirtualBase (), binary->GetBinary ());

		//cpu.Call (binary->GetEntryPoint ());
	}

	return 0;
}
