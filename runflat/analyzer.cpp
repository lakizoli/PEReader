#include "stdafx.h"
#include "analyzer.hpp"
#include <bochs/disasm/disasm.h>

Analyzer::Analyzer (const std::string& flatPath, const std::string& destPath) :
	mFlatPath (flatPath),
	mDestPath (destPath)
{
}

Analyzer::~Analyzer () {
}

void Analyzer::Execute (std::shared_ptr<FlatBinary> binary) {
	//TODO: ...

	//Dump disassembly
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