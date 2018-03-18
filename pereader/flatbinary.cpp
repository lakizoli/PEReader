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
	mStackSize (0),
	mHeapSize (0),
	mCodeSize (0),
	mEntryPoint (0),
	mRelocationBase (0)
{
}

bool FlatBinary::LoadFunctionTable (std::fstream& file, std::map<uint64_t, std::string>& functionTable) {
	uint64_t importCount = 0;
	file.read ((char*) &importCount, sizeof (importCount));
	if (!file) {
		return false;
	}

	for (uint64_t i = 0; i < importCount; ++i) {
		uint64_t virtualAddress = 0;
		file.read ((char*) &virtualAddress, sizeof (virtualAddress));
		if (!file) {
			return false;
		}

		uint16_t nameLength = 0;
		file.read ((char*) &nameLength, sizeof (nameLength));
		if (!file) {
			return false;
		}

		std::string name;
		if (nameLength > 0) {
			name.resize (nameLength);
			file.read (&name[0], nameLength);
			if (!file) {
				return false;
			}
		}

		functionTable.emplace (virtualAddress, name);
	}

	return true;
}

bool FlatBinary::SaveFunctionTable (const std::map<uint64_t, std::string>& functionTable, std::fstream& file) {
	uint64_t exportCount = functionTable.size ();
	file.write ((const char*) &exportCount, sizeof (exportCount));
	if (!file) {
		return false;
	}

	for (auto& it : functionTable) {
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

	return true;
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
		binary->mCodeSize = header->SizeOfCode;
		binary->mEntryPoint = header->AddressOfEntryPoint;
		binary->mRelocationBase = header->ImageBase;

		break;
	}
	case PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR64_MAGIC: {
		binary->mType = BinaryTypes::Bit64;

		const PEOptionalHeader64* header = peHeader.optionalHeader.header64;
		binary->mVirtualBase = header->BaseOfCode;
		binary->mVirtualSize = header->SizeOfImage;
		binary->mStackSize = header->SizeOfStackReserve;
		binary->mHeapSize = header->SizeOfHeapReserve;
		binary->mCodeSize = header->SizeOfCode;
		binary->mEntryPoint = header->AddressOfEntryPoint;
		binary->mRelocationBase = header->ImageBase;

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
	if (!peHeader.EnumerateImports ([binary] (bool delayLoaded, const std::string& moduleName, const std::string& importFunction, uint64_t iatAddress) -> void {
		std::stringstream ss;
		ss << "[" << moduleName << "]" << importFunction;

		if (delayLoaded) { //Delay loaded import functions
			binary->mDelayImports.emplace (iatAddress, ss.str ());
		} else { //Prior start imported functions
			binary->mImports.emplace (iatAddress, ss.str ());
		}
	})) {
		return nullptr;
	}

	//Collect exports
	if (!peHeader.EnumerateExports ([binary] (uint32_t ordinal, const std::string& name, uint64_t virtualAddress) -> void {
		binary->mExports.emplace (virtualAddress, name);
	})) {
		return nullptr;
	}

	//Collect relocations
	if (!peHeader.EnumerateRelocations ([binary] (bool isHighPart, uint16_t wide, uint64_t virtualAddress) -> void {
		RelocationTypes type = RelocationTypes::Unknown;
		switch (wide) {
		case 2:
			type = isHighPart ? RelocationTypes::UInt16High : RelocationTypes::UInt16Low;
			break;
		case 4:
			type = RelocationTypes::UInt32;
			break;
		case 8:
			type = RelocationTypes::UInt64;
			break;
		default:
			break;
		}

		if (type != RelocationTypes::Unknown) {
			auto it = binary->mRelocations.find (type);
			if (it == binary->mRelocations.end ()) {
				binary->mRelocations.emplace (type, std::set<uint64_t> { virtualAddress });
			} else {
				it->second.insert (virtualAddress);
			}
		}
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

	file.read ((char*) &binary->mCodeSize, sizeof (mCodeSize));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mEntryPoint, sizeof (mEntryPoint));
	if (!file) {
		return nullptr;
	}

	file.read ((char*) &binary->mRelocationBase, sizeof (mRelocationBase));
	if (!file) {
		return nullptr;
	}

	//Read imports
	if (!LoadFunctionTable (file, binary->mImports)) {
		return nullptr;
	}

	//Read delayed imports
	if (!LoadFunctionTable (file, binary->mDelayImports)) {
		return nullptr;
	}

	//Read exports
	if (!LoadFunctionTable (file, binary->mExports)) {
		return nullptr;
	}

	//Read relocations
	uint64_t relocationTypeCount = 0;
	file.read ((char*) &relocationTypeCount, sizeof (relocationTypeCount));
	if (!file) {
		return nullptr;
	}

	for (uint64_t i = 0; i < relocationTypeCount; ++i) {
		uint32_t relocationType = 0;
		file.read ((char*) &relocationType, sizeof (relocationType));
		if (!file) {
			return nullptr;
		}

		uint64_t relocationCount = 0;
		file.read ((char*) &relocationCount, sizeof (relocationCount));
		if (!file) {
			return nullptr;
		}

		for (uint64_t j = 0; j < relocationCount; ++j) {
			uint64_t virtualAddress = 0;
			file.read ((char*) &virtualAddress, sizeof (virtualAddress));
			if (!file) {
				return nullptr;
			}

			auto it = binary->mRelocations.find ((RelocationTypes) relocationType);
			if (it == binary->mRelocations.end ()) {
				binary->mRelocations.emplace ((RelocationTypes) relocationType, std::set<uint64_t> { virtualAddress });
			} else {
				it->second.insert (virtualAddress);
			}
		}
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

	file.write ((const char*) &mCodeSize, sizeof (mCodeSize));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mEntryPoint, sizeof (mEntryPoint));
	if (!file) {
		return false;
	}

	file.write ((const char*) &mRelocationBase, sizeof (mRelocationBase));
	if (!file) {
		return false;
	}

	//Write imports
	if (!SaveFunctionTable (mImports, file)) {
		return false;
	}

	//Write delay imports
	if (!SaveFunctionTable (mDelayImports, file)) {
		return false;
	}

	//Write exports
	if (!SaveFunctionTable (mExports, file)) {
		return false;
	}

	//Write relocations
	uint64_t relocationTypeCount = mRelocations.size ();
	file.write ((const char*) &relocationTypeCount, sizeof (relocationTypeCount));
	if (!file) {
		return false;
	}

	for (auto& it : mRelocations) {
		uint32_t relocationType = (uint32_t) it.first;
		file.write ((const char*) &relocationType, sizeof (relocationType));
		if (!file) {
			return false;
		}

		uint64_t relocationCount = it.second.size ();
		file.write ((const char*) &relocationCount, sizeof (relocationCount));
		if (!file) {
			return false;
		}

		for (uint64_t virtualAddress : it.second) {
			file.write ((const char*) &virtualAddress, sizeof (virtualAddress));
			if (!file) {
				return false;
			}
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
