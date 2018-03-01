#pragma once

#include "flatbinary.hpp"
#include "asmfunction.hpp"
#include "asmlink.hpp"

class Analyzer {
	std::string mFlatPath;
	std::string mDestPath;
	std::shared_ptr<FlatBinary> mBinary;
	std::map<uint64_t, std::shared_ptr<ASMLink>> mLinks;
	std::map<uint64_t, std::shared_ptr<ASMFunction>> mFunctions;

	bool IsEndCommand (const std::string& command) const;
	std::tuple<bool, int64_t> ConvertRelativeAddress (const std::string& relativeAddress) const;
	std::tuple<bool, int64_t> CalculateOperand1Offset (const std::string & command) const;
	std::tuple<bool, uint64_t> DetectImportCall (uint64_t targetIP) const;
	bool DetectNullSubCall (uint64_t targetIP) const;
	std::shared_ptr<ASMFunction> WalkFunction (uint64_t address) const;

public:
	Analyzer (const std::string& flatPath, const std::string& destPath);
	~Analyzer () {};

	void Execute (std::shared_ptr<FlatBinary> binary);
	void SaveAssemblyFiles () const;
	void SaveUsedCommandsFile () const;
};

