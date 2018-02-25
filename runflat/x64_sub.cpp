#include "stdafx.h"
#include "x64_sub.hpp"

IMPLEMENT_ASSEMBLY_COMMAND (x64_sub, "sub");

x64_sub::x64_sub () {
}

bool x64_sub::Run (Cpu& cpu, const Cpu::Command& command) {
	std::string opDest = command.cmd[1];
	switch (command.ops[0]) {
	case Cpu::OperandTypes::Register: {
		Register& dest = cpu.GetRegister (opDest);
		uint32_t size = command.opSizes[0];

		//std::string opSrc = GetOperand (1, command.cmd);
		break;
	}
	case Cpu::OperandTypes::Memory: {
		//TODO: ...
		break;
	}
	default:
		break;
	}

	return false;
}
