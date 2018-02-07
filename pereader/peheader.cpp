#include "stdafx.h"
#include "peheader.hpp"

PEHeader::PEHeader () {
}

bool PEHeader::EnumerateImports (ImportCallback callback) const {
	const PEDataDirectory& importDirectory = optionalHeader.dataDirectories[(uint16_t) PEOptionalHeader::DirectoryEntries::IMAGE_DIRECTORY_ENTRY_IMPORT];
	const PEImportDescriptor* descriptor = sectionTable.GetInstanceOnVirtualAddress<PEImportDescriptor> (importDirectory.VirtualAddress);
	while (descriptor && descriptor->Characteristics != 0) {
		const char* moduleName = sectionTable.GetInstanceOnVirtualAddress<char> (descriptor->Name);
		if (moduleName) {
			if (optionalHeader.type == PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR32_MAGIC) { //32Bit executable
				WalkImportFunctions<uint32_t> (false, descriptor->FirstThunk, moduleName, callback);
			} else if (optionalHeader.type == PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR64_MAGIC) { //64Bit executable
				WalkImportFunctions<uint64_t> (false, descriptor->FirstThunk, moduleName, callback);
			} else { //Unhandled import format
				return false;
			}
		}
		++descriptor;
	}

	const PEDataDirectory& importDirectoryDelay = optionalHeader.dataDirectories[(uint16_t) PEOptionalHeader::DirectoryEntries::IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
	const PEDelayImportDescriptor* descriptorDelay = sectionTable.GetInstanceOnVirtualAddress<PEDelayImportDescriptor> (importDirectoryDelay.VirtualAddress);
	while (descriptorDelay && descriptorDelay->ModuleHandleRVA != 0 && descriptorDelay->DllNameRVA != 0) {
		if (descriptorDelay->Attributes.RvaBased) {
			const char* moduleName = sectionTable.GetInstanceOnVirtualAddress<char> (descriptorDelay->DllNameRVA);
			if (moduleName) {
				if (optionalHeader.type == PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR32_MAGIC) { //32Bit executable
					WalkImportFunctions<uint32_t> (true, descriptorDelay->ImportNameTableRVA, moduleName, callback);
				} else if (optionalHeader.type == PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR64_MAGIC) { //64Bit executable
					WalkImportFunctions<uint64_t> (true, descriptorDelay->ImportNameTableRVA, moduleName, callback);
				} else { //Unhandled import format
					return false;
				}
			}
		} else { //Unhandled import format
			return false;
		}
		++descriptorDelay;
	}

	return true;
}

std::istream& operator>> (std::istream& stream, PEHeader& header) {
	//Read DOS header
	stream >> header.dosHeader;
	if (!stream) {
		return stream;
	}

	if (!header.dosHeader.IsValid ()) {
		stream.setstate (std::ios_base::failbit);
		return stream;
	}

	//Seek to PE file header
	stream.seekg (header.dosHeader.e_lfanew, std::ios_base::beg);
	if (!stream) {
		return stream;
	}

	//Read PE file header
	stream >> header.fileHeader;
	if (!stream) {
		return stream;
	}

	if (!header.fileHeader.IsValid ()) {
		stream.setstate (std::ios_base::failbit);
		return stream;
	}

	//Read optional header
	stream >> header.optionalHeader;
	if (!stream) {
		return stream;
	}

	//Read section table
	PESectionTable::Read (stream, header.fileHeader.NumberOfSections, header.sectionTable);
	if (!stream) {
		return stream;
	}

	return stream;
}
