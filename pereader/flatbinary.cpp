#include "stdafx.h"
#include "flatbinary.hpp"

#ifdef FLATBIN_COMPLIE_GENERATION
#	include "pe.hpp"
#endif //FLATBIN_COMPLIE_GENERATION

FlatBinary::FlatBinary () :
	mVersion (1),
	mType (BinaryTypes::Unknown),
	mVirtualBase (0),
	mVirtualSize (0),
	mEntryPoint (0),
	mImportTableAddress (0)
{
}

#ifdef FLATBIN_COMPLIE_GENERATION

std::shared_ptr<FlatBinary> FlatBinary::Create (const PE& pe) {
	std::shared_ptr<FlatBinary> binary (new FlatBinary ());

	//Compose header
	const PEHeader& peHeader = pe.GetHeader ();

	switch (peHeader.optionalHeader.type) {
	case PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR32_MAGIC: {
		binary->mType = BinaryTypes::Bit32;

		const PEOptionalHeader32* header = peHeader.optionalHeader.header32;
		binary->mVirtualBase = header->BaseOfCode;
		binary->mVirtualSize = header->SizeOfImage;
		binary->mStackSize = header->SizeOfStackReserve;
		binary->mHeapSize = header->SizeOfHeapReserve;
		binary->mEntryPoint = header->AddressOfEntryPoint;

		break;
	}
	case PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR64_MAGIC: {
		binary->mType = BinaryTypes::Bit64;

		const PEOptionalHeader64* header = peHeader.optionalHeader.header64;
		binary->mVirtualBase = header->BaseOfCode;
		binary->mVirtualSize = header->SizeOfImage;
		binary->mStackSize = header->SizeOfStackReserve;
		binary->mHeapSize = header->SizeOfHeapReserve;
		binary->mEntryPoint = header->AddressOfEntryPoint;

		break;
	}
	default:
		return nullptr;
	}

	//Compose binary
	uint64_t binaryStartOffset = UINT64_MAX;
	for (size_t i = 0, iEnd = peHeader.sectionTable.headers.size (); i < iEnd; ++i) {
		if (peHeader.sectionTable.headers[i].VirtualAddress < binaryStartOffset) {
			binaryStartOffset = peHeader.sectionTable.headers[i].VirtualAddress;
		}
	}

	if (binaryStartOffset != binary->mVirtualBase) { //Some region filled below the virtual base address space, so we have to exit with error...
		return nullptr;
	}

	binary->mBinary.resize (binary->mVirtualSize - binaryStartOffset);
	for (size_t i = 0, iEnd = peHeader.sectionTable.headers.size (); i < iEnd; ++i) {
		const PESectionHeader& sectionHeader = peHeader.sectionTable.headers[i];
		const PESection& section = peHeader.sectionTable.sections[i];

		uint64_t pos = (uint64_t) sectionHeader.VirtualAddress - binaryStartOffset;
		std::copy (section.data.begin (), section.data.end (), binary->mBinary.begin () + pos);
	}

	//Collect imports
	const PEDataDirectory& iat = peHeader.optionalHeader.dataDirectories[(uint16_t) PEOptionalHeader::DirectoryEntries::IMAGE_DIRECTORY_ENTRY_IAT];
	binary->mImportTableAddress = iat.VirtualAddress;

	if (!peHeader.EnumerateImports ([binary] (bool delayLoaded, const std::string& moduleName, const std::string& importFunction, uint64_t iatAddress) -> void {
		uint64_t iatOffset = iatAddress - binary->mImportTableAddress;

		std::stringstream ss;
		ss << "[" << moduleName << "]" << importFunction;
		binary->mImports.emplace (iatOffset, ss.str ());
	})) {
		return nullptr;
	}

	//Collect exports
	if (!peHeader.EnumerateExports ([binary] (uint32_t ordinal, const std::string& name, uint64_t virtualAddress) -> void {
		binary->mExports.emplace (virtualAddress, name);
	})) {
		return nullptr;
	}

	return binary;
}

#endif //FLATBIN_COMPLIE_GENERATION

std::shared_ptr<FlatBinary> FlatBinary::Load (const std::string& path) {
	std::shared_ptr<FlatBinary> binary (new FlatBinary ());

	std::fstream file (path, std::ios::binary | std::ios::in);
	if (!file) {
		return false;
	}

	//Read magic
	uint8_t magic[4] = { 'Z', 'F', 'B', '\0' };
	uint8_t magicData[4] = { 0,0,0,0 };
	file.read ((char*) magicData, 4 * sizeof (uint8_t));
	if (!file) {
		return nullptr;
	}

	if (std::memcmp (magic, magicData, 4 * sizeof (uint8_t)) != 0) {
		return nullptr;
	}

	//Read header
	file.read ((char*) &binary->mVersion, sizeof (mVersion));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mType, sizeof (mType));
	if (!file) {
		return nullptr;
	}

	uint32_t reserved = 0; //align to 64 bit
	file.read ((char*) &reserved, sizeof (reserved));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mVirtualBase, sizeof (mVirtualBase));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mVirtualSize, sizeof (mVirtualSize));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mStackSize, sizeof (mStackSize));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mHeapSize, sizeof (mHeapSize));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mEntryPoint, sizeof (mEntryPoint));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mImportTableAddress, sizeof (mImportTableAddress));
	if (!file) {
		return nullptr;
	}

	//Read imports
	uint64_t importCount = 0;
	file.read ((char*) &importCount, sizeof (importCount));
	if (!file) {
		return nullptr;
	}

	uint64_t iatItemSize = 0;
	switch (binary->mType) {
	case BinaryTypes::Bit32:
		iatItemSize = 4;
		break;
	case BinaryTypes::Bit64:
		iatItemSize = 8;
		break;
	default:
		return nullptr;
	}

	std::string lastModuleName;
	uint64_t pos = 0;
	for (uint64_t i = 0; i < importCount; ++i) {
		uint16_t nameLength = 0;
		file.read ((char*) &nameLength, sizeof (nameLength));
		if (!file) {
			return nullptr;
		}

		bool havePosShift = false;
		std::string name;
		if (nameLength > 0) {
			name.resize (nameLength);
			file.read (&name[0], nameLength);
			if (!file) {
				return nullptr;
			}

			if (name[0] == '+') {
				havePosShift = true;
				name = name.substr (1);
			}
		}

		if (havePosShift) {
			pos += iatItemSize; //Ignore null item at module ends in IAT
		}

		binary->mImports.emplace (pos, name);
		pos += iatItemSize;
	}

	//Read exports
	uint64_t exportCount = 0;
	file.read ((char*) &exportCount, sizeof (exportCount));
	if (!file) {
		return nullptr;
	}

	for (uint64_t i = 0; i < exportCount; ++i) {
		uint64_t exportAddress = 0;
		file.read ((char*) &exportAddress, sizeof (exportAddress));
		if (!file) {
			return nullptr;
		}

		uint16_t nameLength = 0;
		file.read ((char*) &nameLength, sizeof (nameLength));
		if (!file) {
			return nullptr;
		}

		std::string name;
		if (nameLength > 0) {
			name.resize (nameLength);
			file.read (&name[0], nameLength);
			if (!file) {
				return nullptr;
			}
		}

		binary->mExports.emplace (exportAddress, name);
	}

	//Read data
	uint64_t binaryCount = 0;
	file.read ((char*) &binaryCount, sizeof (binaryCount));
	if (!file) {
		return false;
	}

	if (binaryCount > 0) {
		binary->mBinary.resize (binaryCount);

		file.read ((char*) &binary->mBinary[0], binaryCount * sizeof (uint8_t));
		if (!file) {
			return nullptr;
		}
	}

	return binary;
}

bool FlatBinary::Save (const std::string& path) {
	std::fstream file (path, std::ios::binary | std::ios::out | std::ios::trunc);
	if (!file) {
		return false;
	}

	//Write magic
	uint8_t magic[4] = { 'Z', 'F', 'B', '\0' };
	file.write ((const char*) magic, 4 * sizeof (uint8_t));
	if (!file) {
		return false;
	}

	//Write header
	file.write ((const char*) &mVersion, sizeof (mVersion));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mType, sizeof (mType));
	if (!file) {
		return false;
	}

	uint32_t reserved = 0; //align to 64 bit
	file.write ((const char*) &reserved, sizeof (reserved));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mVirtualBase, sizeof (mVirtualBase));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mVirtualSize, sizeof (mVirtualSize));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mStackSize, sizeof (mStackSize));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mHeapSize, sizeof (mHeapSize));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mEntryPoint, sizeof (mEntryPoint));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mImportTableAddress, sizeof (mImportTableAddress));
	if (!file) {
		return false;
	}

	//Write imports
	uint64_t importCount = mImports.size ();
	file.write ((const char*) &importCount, sizeof (importCount));
	if (!file) {
		return false;
	}

	std::string lastModuleName;
	for (auto& it : mImports) {
		//Write offset shift
		bool haveOffsetShift = false;

		size_t posModuleNameEnd = it.second.find (']');
		if (posModuleNameEnd != std::string::npos) {
			std::string moduleName = it.second.substr (0, posModuleNameEnd + 1);
			if (!lastModuleName.empty () && moduleName != lastModuleName) {
				haveOffsetShift = true;
			}
			lastModuleName = moduleName;
		}

		//Write module name with offset shift sign
		uint16_t nameLength = (uint16_t) it.second.length ();
		if (haveOffsetShift) {
			++nameLength;
		}

		file.write ((const char*) &nameLength, sizeof (nameLength));
		if (!file) {
			return false;
		}

		if (haveOffsetShift) {
			file.write ("+", 1);
			if (!file) {
				return false;
			}
		}

		file.write (it.second.c_str (), it.second.length ());
		if (!file) {
			return false;
		}
	}

	//Write exports
	uint64_t exportCount = mExports.size ();
	file.write ((const char*) &exportCount, sizeof (exportCount));
	if (!file) {
		return false;
	}

	for (auto& it : mExports) {
		//Write export's virtual address
		file.write ((const char*) &it.first, sizeof (it.first));
		if (!file) {
			return false;
		}

		//Write export name
		uint16_t nameLength = (uint16_t) it.second.length ();
		file.write ((const char*) &nameLength, sizeof (nameLength));
		if (!file) {
			return false;
		}

		file.write (it.second.c_str (), nameLength);
		if (!file) {
			return false;
		}
	}

	//Write data
	uint64_t binaryCount = mBinary.size ();
	file.write ((const char*) &binaryCount, sizeof (binaryCount));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mBinary[0], binaryCount * sizeof (uint8_t));
	if (!file) {
		return false;
	}

	return true;
}
