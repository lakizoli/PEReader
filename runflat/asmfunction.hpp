#pragma once
#include "asmlink.hpp"

class ASMFunction {
	std::string mName; ///< The name of the function
	uint64_t mVirtualAddress; ///< The virtual address of the function
	uint64_t mLength; ///< The length of the function in bytes
	std::map<uint64_t, std::shared_ptr<ASMLink>> mLinksBySourceAddress; ///< Links originated from this function assigned to source address
	std::map<uint64_t, std::vector<std::shared_ptr<ASMLink>>> mLinksByTargetAddress; ///< Links originated from this function assigned to target address
	std::map<uint64_t, std::string> mAsmSource; ///< The ASM source code of this function assigned to the ip value.

public:
	ASMFunction (uint64_t virtualAddress, uint64_t length, const std::vector<std::shared_ptr<ASMLink>>& links,
		const std::map<uint64_t, std::string>& asmSource, const std::string& namePrefix = std::string ());

	~ASMFunction () {}

	bool IsAddressInRange (uint64_t address) const {
		return address >= mVirtualAddress && address < mVirtualAddress + mLength;
	}

	std::set<std::string> GatherUsedAssemblyCommands () const;

public:
	void SetName (const std::string& name) {
		mName = name;
	}

	const std::string& GetName () const {
		return mName;
	}

	uint64_t GetVirtualAddress () const {
		return mVirtualAddress;
	}

	uint64_t GetLength () const {
		return mLength;
	}

	const std::map<uint64_t, std::shared_ptr<ASMLink>>& GetLinksBySourceAddress () const {
		return mLinksBySourceAddress;
	}

	const std::map<uint64_t, std::vector<std::shared_ptr<ASMLink>>>& GetLinksByTargetAddress () const {
		return mLinksByTargetAddress;
	}

	const std::map<uint64_t, std::string>& GetAsmSource () const {
		return mAsmSource;
	}
};
