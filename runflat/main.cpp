// runflat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <flatbinary.hpp>
#include <bochs/bochs.h>
#include <bochs/cpu/cpu.h>
#include <bochs/memory/memory-bochs.h>
#include <bochs/disasm/disasm.h>

static int PrintUsage (const char* exe) {
	std::cout <<
		"Usage:" << std::endl <<
		"    " << std::experimental::filesystem::path (exe).filename ().string () << " <flat path>" << std::endl <<
		std::endl;

	return 1;
}

extern int bx_init_main (int argc, char *argv[]);

int CDECL main (int argc, char* argv[]) {
	if (argc < 2) {
		return PrintUsage (argc > 0 ? argv[0] : "runflat");
	}

	std::shared_ptr<FlatBinary> binary = FlatBinary::Load (argv[1]);
	if (binary == nullptr) {
		std::cout << "Error: cannot load flat binary from file at path: " << argv[1] << std::endl << std::endl;
		return 1;
	}

	//TEST Disassembly
	//{
	//	const std::vector<uint8_t>& data = binary->GetBinary ();
	//	uint64_t entryPoint = binary->GetEntryPoint ();
	//	uint64_t virtualBase = binary->GetVirtualBase ();
	//	disassembler dis;

	//	char commandBuffer[1024];
	//	const uint8_t* ip = &data[entryPoint - virtualBase];
	//	uint32_t res = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
	//	while (true) {
	//		ip += res;
	//		res = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
	//	}
	//}
	//END TEST Disassembly

	//Run program
	{
		bx_init_siminterface ();
		if (bx_init_main (1, argv) < 0) {
			BX_INSTR_EXIT_ENV ();
			return 0;
		}

		SIM->get_param_enum ("cpu.model")->set (bx_cpudb_corei7_sandy_bridge_2600k);

		//std::shared_ptr<BX_MEM_C> mem (new BX_MEM_C ());
		//std::shared_ptr<BX_CPU_C> cpu (new BX_CPU_C ());

		uint64_t memSize = 16 * 1024 * 1024; //Define 16 mb memory in virtual environment
		uint64_t hostMemSize = 16 * 1024 * 1024; //Allocate 16 mb memory on host computer
		BX_MEM (0)->init_memory (memSize, hostMemSize);

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

			uint8_t* ptr = BX_MEM (0)->get_vector (virtualBase + pos);
			memcpy (ptr, &data[pos], len);

			pos += len;
		}

		BX_CPU (0)->initialize ();
		BX_CPU (0)->sanity_checks ();
		BX_CPU (0)->register_state ();
		//BX_INSTR_INITIALIZE (0);

		uint64_t startAddress = binary->GetEntryPoint () - virtualBase;

		//TODO: ...
	}

    return 0;
}
