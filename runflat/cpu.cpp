#include "stdafx.h"
#include "cpu.hpp"
#include "assemblycommand.hpp"
#include <bochs/disasm/disasm.h>

Cpu::Command Cpu::ReadCommand (uint64_t virtualAddress) const {
	disassembler dis;
	char commandBuffer[1024];

	const uint8_t* ip = &mBinary[virtualAddress - mVirtualBase];

	Command result;
	result.length = dis.disasm64 (mVirtualBase, mVirtualBase, ip, commandBuffer);

	//Store command items
	std::string item;
	char* ptr = commandBuffer;
	while (*ptr) {
		switch (*ptr) {
		case ' ':
			if (result.cmd.size () > 0) { //This is only a whitespace
				if (item.size () > 0) { //Ignore leading whitespaces...
					item.push_back (*ptr);
				}
			} else { //This is the command
				result.cmd.push_back (item);
				item.clear ();
			}
			break;
		case ',':
			result.cmd.push_back (item);
			item.clear ();
			break;
		default:
			item.push_back (*ptr);
			break;
		}

		++ptr;
	}

	if (item.size () > 0) {
		result.cmd.push_back (item);
	}

	//Determine type of operands
	for (size_t i = 1, iEnd = result.cmd.size();i < iEnd;++i) {
		const std::string& operand = result.cmd[i];

		if (std::isdigit (operand[0]) || operand[0] == '.' || operand[0] == '-' || operand[0] == '+') {
			result.ops.push_back (OperandTypes::Immediate);
		} else if (operand[0] == '[') {
			result.ops.push_back (OperandTypes::Memory);
		} else {
			result.ops.push_back (OperandTypes::Register);
		}
	}

	//Determine size of operands
	for (size_t i = 0, iEnd = result.ops.size (); i < iEnd; ++i) {
		const std::string& operand = result.cmd[i + 1];

		switch (result.ops[i]) {
		case OperandTypes::Immediate:
			result.opSizes.push_back (sizeof (uint64_t));
			break;
		case OperandTypes::Memory:
			//TODO: ...
			break;
		case OperandTypes::Register: //Calculate register size
			if (operand[0] == 'r') {
				switch (operand[operand.length () - 1]) {
				case 'b':
					result.opSizes.push_back (sizeof (uint8_t));
					break;
				case 'w':
					result.opSizes.push_back (sizeof (uint16_t));
					break;
				case 'd':
					result.opSizes.push_back (sizeof (uint32_t));
					break;
				default:
					result.opSizes.push_back (sizeof (uint64_t));
					break;
				}
			} else if (operand[0] == 'e') {
				result.opSizes.push_back (sizeof (uint32_t));
			} else if (operand.size () == 2 || operand.size () == 3) { //8 or 16 bit register
				switch (operand[operand.length () - 1]) {
				case 'h':
				case 'l':
					result.opSizes.push_back (sizeof (uint8_t));
					break;
				default:
					result.opSizes.push_back (sizeof (uint16_t));
					break;
				}
			}
			break;
		default:
			result.opSizes.push_back (sizeof (uint64_t));
			break;
		}
	}

	return result;
}

Cpu::Cpu () :
	mVirtualBase (0),
	rax ("rax"),
	rbx ("rbx"),
	rcx ("rcx"),
	rdx ("rdx"),
	rsi ("rsi"),
	rdi ("rdi"),
	rbp ("rbp"),
	rsp ("rsp"),
	r8 ("r8"),
	r9 ("r9"),
	r10 ("r10"),
	r11 ("r11"),
	r12 ("r12"),
	r13 ("r13"),
	r14 ("r14"),
	r15 ("r15"),
	rip ("rip")
{
}

bool Cpu::Call (uint64_t virtualAddress) {
	//Init stack
	memset (&mStack[0], 0, mStack.size ());

	//Init cpu
	rax.SetQW (0);
	rbx.SetQW (0);
	rcx.SetQW (0);
	rdx.SetQW (0);
	rsi.SetQW (0);
	rdi.SetQW (0);
	rbp.SetQW (0);
	rsp.SetQW (mStackVirtualAddress + mStack.size ());
	r8.SetQW (0);
	r9.SetQW (0);
	r10.SetQW (0);
	r11.SetQW (0);
	r12.SetQW (0);
	r13.SetQW (0);
	r14.SetQW (0);
	r15.SetQW (0);

	rip.SetQW (virtualAddress);

	//Run the program
	while (true) {
		//Get next comand
		uint64_t ip = rip.GetQW ();
		Command command = ReadCommand (ip);
		rip.SetQW (ip + command.length);

		//Execute command
		std::shared_ptr<AssemblyCommand> assembly = AssemblyCommand::Create (command);
		if (assembly == nullptr) {
			return false;
		}

		if (!assembly->Run (*this, command)) {
			return false;
		}

		//TODO: ...
	}

	return true;
}

Register& Cpu::GetRegister (const std::string& name) {
	if (name.length () == 2) {
		switch (name[0]) {
		case 'a': return rax;
		case 'b': return name[1] == 'p' ? rbp : rbx;
		case 'c': return rcx;
		case 'd': return name[1] == 'i' ? rdi : rdx;
		case 'i': return rip;
		case 's': return name[1] == 'i' ? rsi : rsp;
		case '8': return r8;
		case '9': return r9;
		default:
			break;
		}
	} else if (name.length () >= 3) {
		switch (name[1]) {
		case 'a': return rax;
		case 'b': return name[2] == 'p' ? rbp : rbx; //rbp, rbx
		case 'c': return rcx;
		case 'd': return name[2] == 'i' ? rdi : rdx; //rdi, rdx
		case 'i': return rip;
		case 'p': return name[0] == 'b' ? rbp : rsp; //bpl, spl
		case 's': return name[2] == 'i' ? rsi : rsp; //rsi, rsp
		case '1': {
			switch (name[2]) {
			case '0': return r10;
			case '1': return r11;
			case '2': return r12;
			case '3': return r13;
			case '4': return r14;
			case '5': return r15;
			default:
				break;
			}
		}
		case '8': return r8;
		case '9': return r9;
		default:
			break;
		}
	}

	return rax; //error case
}
