#include "stdafx.h"
#include "x64_sub.hpp"
#include "cpu_flags.hpp"

IMPLEMENT_ASSEMBLY_COMMAND (x64_sub, "sub");

x64_sub::x64_sub () {
}

bool x64_sub::Run (Cpu& cpu, const Cpu::Command& command) {
	std::string opDest = command.cmd[1];
	switch (command.ops[0]) { //Destination type
	case Cpu::OperandTypes::Register: {
		Register& dest = cpu.GetRegister (opDest);
		switch (command.opSizes[0]) {
		case 1: {
			uint8_t op2 = 0;
			if (!ExecuteUnsignedOperand<uint8_t, uint8_t (0), uint8_t (0xff)> (command, 1, op2)) {
				return false;
			}

			uint8_t op1 = dest.GetLB ();
			uint8_t diff = op1 - op2;
			dest.SetLB (diff);

			SET_FLAGS_OSZAPC_SUB_8 (cpu, op1, op2, diff);
			return true;
		}
		case 2: {
			uint16_t op2 = 0;
			if (!ExecuteUnsignedOperand<uint16_t, uint16_t (0), uint16_t (0xffff)> (command, 1, op2)) {
				return false;
			}

			uint16_t op1 = dest.GetW ();
			uint16_t diff = op1 - op2;
			dest.SetW (diff);

			SET_FLAGS_OSZAPC_SUB_16 (cpu, op1, op2, diff);
			return true;
		}
		case 4: {
			uint32_t op2 = 0;
			if (!ExecuteUnsignedOperand<uint32_t, uint32_t (0), uint32_t (0xff)> (command, 1, op2)) {
				return false;
			}

			uint32_t op1 = dest.GetDW ();
			uint32_t diff = op1 - op2;
			dest.SetDW (diff);

			SET_FLAGS_OSZAPC_SUB_32 (cpu, op1, op2, diff);
			return true;
		}
		case 8: {
			uint64_t op2 = 0;
			if (!ExecuteUnsignedOperand<uint64_t, uint64_t (0), uint64_t (0xff)> (command, 1, op2)) {
				return false;
			}

			uint64_t op1 = dest.GetQW ();
			uint64_t diff = op1 - op2;
			dest.SetQW (diff);

			SET_FLAGS_OSZAPC_SUB_64 (cpu, op1, op2, diff);
			return true;
		}
		default:
			break;
		}
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
