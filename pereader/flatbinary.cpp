#include "stdafx.h"
#include "flatbinary.hpp"
#include "pe.hpp"

FlatBinary::FlatBinary () :
	mVersion (1),
	mType (BinaryTypes::Unknown),
	mVirtualBase (0),
	mVirtualSize (0),
	mEntryPoint (0),
	mImportTableAddress (0)
{
}

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
	binary->mBinary.resize (binary->mVirtualSize);
	for (size_t i = 0, iEnd = peHeader.sectionTable.headers.size (); i < iEnd; ++i) {
		const PESectionHeader& sectionHeader = peHeader.sectionTable.headers[i];
		const PESection& section = peHeader.sectionTable.sections[i];

		uint32_t pos = sectionHeader.VirtualAddress;
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

	for (auto& it : mImports) {
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

	//Write exports
	//TODO: ...

	//Write data
	file.write ((const char*) &mBinary[0], mBinary.size () * sizeof (uint8_t));
	if (!file) {
		return false;
	}

	return true;
}
