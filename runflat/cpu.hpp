#pragma once
#include "register.hpp"

class Cpu {
public:
	enum class OperandTypes {
		Unknown,
		Register,
		Memory,
		Immediate
	};

	struct Command {
		std::vector<std::string> cmd; ///< The command items
		uint32_t length; ///< The binary command length in bytes
		std::vector<OperandTypes> ops; ///< The types of operands in the command
		std::vector<uint8_t> opSizes; ///< The sizes of operands in the command

		Command () : length (0) {}
	};

private:
	//Executable data
	uint64_t mVirtualBase;
	std::vector<uint8_t> mBinary;

	//Stack
	uint64_t mStackVirtualAddress;
	std::vector<uint8_t> mStack;

	//Processor data
	Register rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15;
	Register rip;

	//Internal functions
	Command ReadCommand (uint64_t virtualAddress) const;

public:
	Cpu ();
	~Cpu () = default;

	bool Call (uint64_t virtualAddress);

public:
	void SetFlatBinary (uint64_t virtualBase, const std::vector<uint8_t>& binary) {
		mVirtualBase = virtualBase;
		mBinary = binary;
	}

	const std::vector<uint8_t>& GetFlatBinary () const {
		return mBinary;
	}

	void InitStack (uint64_t virtualAddress, uint64_t size) {
		mStackVirtualAddress = virtualAddress;
		mStack.resize (size);
	}

	const std::vector<uint8_t>& GetStack () const {
		return mStack;
	}

	Register& GetRegister (const std::string& name);
};

