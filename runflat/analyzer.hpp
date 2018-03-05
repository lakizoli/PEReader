#pragma once

#include "flatbinary.hpp"
#include "asmfunction.hpp"
#include "asmlink.hpp"

class Analyzer {
	struct Operand1Address {
		bool foundValidAddress;
		bool isAbsoluteAddress;
		bool isDynamicCall;
		union {
			int64_t offsetValue;
			uint64_t absoluteValue;
		};

		Operand1Address ();
		explicit Operand1Address (uint64_t absoluteAddress);
		Operand1Address (bool isDynamicCall, int64_t offsetValue);
	};

	std::string mFlatPath;
	std::string mDestPath;
	std::shared_ptr<FlatBinary> mBinary;
	std::map<uint64_t, std::shared_ptr<ASMLink>> mLinks;
	std::map<uint64_t, std::shared_ptr<ASMFunction>> mFunctions;

	bool IsEndCommand (const std::string& command) const;
	std::tuple<bool, int64_t> ConvertRelativeAddress (const std::string& relativeAddress) const;
	Operand1Address CalculateOperand1Address (const std::string & command) const;
	static bool IsImportCall (uint64_t targetIP, const std::map<uint64_t, std::string>& imports);
	bool IsDataSegmentCall (uint64_t targetIP) const;
	bool DetectNullSubCall (uint64_t targetIP) const;
	std::shared_ptr<ASMFunction> WalkFunction (uint64_t address, const std::string& name) const;

public:
	Analyzer (const std::string& flatPath, const std::string& destPath);
	~Analyzer () {};

	void Execute (std::shared_ptr<FlatBinary> binary);
	void SaveAssemblyFiles () const;
	void SaveUsedCommandsFile () const;
};

