// runflat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <flatbinary.hpp>
#include <bochs/disasm/disasm.h>

static int PrintUsage (const char* exe) {
	std::cout <<
		"Usage:" << std::endl <<
		"    " << std::experimental::filesystem::path (exe).filename ().string () << " <flat path>" << std::endl <<
		std::endl;

	return 1;
}

int main (int argc, char* argv[]) {
	if (argc < 2) {
		return PrintUsage (argc > 0 ? argv[0] : "runflat");
	}

	std::shared_ptr<FlatBinary> binary = FlatBinary::Load (argv[1]);
	if (binary == nullptr) {
		std::cout << "Error: cannot load flat binary from file at path: " << argv[1] << std::endl << std::endl;
		return 1;
	}

	{
		const std::vector<uint8_t>& data = binary->GetBinary ();
		uint64_t entryPoint = binary->GetEntryPoint ();
		uint64_t virtualBase = binary->GetVirtualBase ();
		disassembler dis;

		char commandBuffer[1024];
		const uint8_t* ip = &data[entryPoint - virtualBase];
		uint32_t res = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
		while (true) {
			ip += res;
			res = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
		}
	}

	//TODO: ...

    return 0;
}

