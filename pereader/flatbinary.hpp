#pragma once

class PE;

class FlatBinary {
public:
	enum class BinaryTypes : uint32_t {
		Unknown,
		Bit32,
		Bit64,
	};

private:
	uint32_t mVersion; ///< The format version of the flat binary.
	BinaryTypes mType; ///< The type of the executable.
	uint64_t mVirtualBase; ///< The virtual base address of the executable.
	uint64_t mVirtualSize; ///< The size of the flat binary in the virtual address space.
	uint64_t mStackSize; ///< The size of the stack have to be allocated.
	uint64_t mHeapSize; ///< The size of the heap have to be allocated.
	uint64_t mEntryPoint; ///< The entry point of the executable.
	uint64_t mImportTableAddress; ///< The virtual address of the import table in the flat binary. (Take care: item size in address table depends on binary type! 32 bit executables have 4 byte item size, and 64 bit executables have 8 byte item size!)
	std::map<uint64_t, std::string> mImports; ///< The imports stored in order of the address table. (format: "[module]function")
	std::vector<uint8_t> mBinary; ///< The flat binary.

	FlatBinary ();

public:
	static std::shared_ptr<FlatBinary> Create (const PE& pe);

	bool Save (const std::string& path);
};
