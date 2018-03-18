#include "stdafx.h"
#include "peheader.hpp"
#include "perelocation.hpp"

PEHeader::PEHeader () {
}

bool PEHeader::EnumerateImports (ImportCallback callback) const {
	const PEDataDirectory& importDirectory = optionalHeader.dataDirectories[(uint16_t) PEOptionalHeader::DirectoryEntries::IMAGE_DIRECTORY_ENTRY_IMPORT];
	const PEImportDescriptor* descriptor = sectionTable.GetInstanceOnVirtualAddress<PEImportDescriptor> (importDirectory.VirtualAddress);
	while (descriptor && descriptor->Characteristics != 0) {
		const char* moduleName = sectionTable.GetInstanceOnVirtualAddress<char> (descriptor->Name);
		if (moduleName) {
			if (optionalHeader.type == PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR32_MAGIC) { //32Bit executable
				WalkImportFunctions<uint32_t> (false, descriptor->OriginalFirstThunk, descriptor->FirstThunk, moduleName, callback);
			} else if (optionalHeader.type == PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR64_MAGIC) { //64Bit executable
				WalkImportFunctions<uint64_t> (false, descriptor->OriginalFirstThunk, descriptor->FirstThunk, moduleName, callback);
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
					WalkImportFunctions<uint32_t> (true, descriptorDelay->ImportNameTableRVA, descriptorDelay->ImportAddressTableRVA, moduleName, callback);
				} else if (optionalHeader.type == PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR64_MAGIC) { //64Bit executable
					WalkImportFunctions<uint64_t> (true, descriptorDelay->ImportNameTableRVA, descriptorDelay->ImportAddressTableRVA, moduleName, callback);
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

bool PEHeader::EnumerateExports (ExportCallback callback) const {
	const PEDataDirectory& exportDirectory = optionalHeader.dataDirectories[(uint16_t) PEOptionalHeader::DirectoryEntries::IMAGE_DIRECTORY_ENTRY_EXPORT];
	const PEExportDescriptor* descriptor = sectionTable.GetInstanceOnVirtualAddress<PEExportDescriptor> (exportDirectory.VirtualAddress);
	if (descriptor) {
		if (descriptor->NumberOfNames != descriptor->NumberOfFunctions) { //Ensure equality
			return false;
		}

		const uint32_t* funcTable = sectionTable.GetInstanceOnVirtualAddress<uint32_t> (descriptor->AddressOfFunctions);
		if (funcTable == nullptr) {
			return false;
		}

		const uint32_t* nameTable = sectionTable.GetInstanceOnVirtualAddress<uint32_t> (descriptor->AddressOfNames);
		if (nameTable == nullptr) {
			return false;
		}

		const uint16_t* ordinalTable = sectionTable.GetInstanceOnVirtualAddress<uint16_t> (descriptor->AddressOfNameOrdinals);
		if (ordinalTable == nullptr) {
			return false;
		}

		for (uint32_t i = 0; i < descriptor->NumberOfFunctions; ++i) {
			const char* funcName = sectionTable.GetInstanceOnVirtualAddress<char> (nameTable[i]);
			if (funcName) {
				callback (descriptor->Base + ordinalTable[i], std::string (funcName), funcTable[i]);
			}
		}
	}

	return true;
}

bool PEHeader::EnumerateRelocations (RelocationCallback callback) const {
	const PEDataDirectory& baseRelocationDirectory = optionalHeader.dataDirectories[(uint16_t) PEOptionalHeader::DirectoryEntries::IMAGE_DIRECTORY_ENTRY_BASERELOC];
	const PEBaseRelocation* baseRelocation = sectionTable.GetInstanceOnVirtualAddress<PEBaseRelocation> (baseRelocationDirectory.VirtualAddress);
	while (baseRelocation && baseRelocation->SizeOfBlock > 0) {
		uint32_t countOfRelocations = (baseRelocation->SizeOfBlock - sizeof (PEBaseRelocation)) / sizeof (uint16_t);
		PEBaseRelocationValue* relocValues = (PEBaseRelocationValue*) (((const uint8_t*) baseRelocation) + sizeof (PEBaseRelocation));
		for (uint32_t i = 0; i < countOfRelocations; ++i) {
			PEBaseRelocationValue relocValue = relocValues[i];

			switch ((PERelocationType) relocValue.type) {
			case PERelocationType::IMAGE_REL_BASED_HIGH:
				callback (true, 2, baseRelocation->VirtualAddress + relocValue.offset);
				break;
			case PERelocationType::IMAGE_REL_BASED_LOW:
				callback (false, 2, baseRelocation->VirtualAddress + relocValue.offset);
				break;
			case PERelocationType::IMAGE_REL_BASED_HIGHLOW:
				callback (false, 4, baseRelocation->VirtualAddress + relocValue.offset);
				break;
			case PERelocationType::IMAGE_REL_BASED_DIR64:
				callback (false, 8, baseRelocation->VirtualAddress + relocValue.offset);
				break;
			default:
				break;
			}
		}

		baseRelocation = (const PEBaseRelocation*) ((uint8_t*) baseRelocation + baseRelocation->SizeOfBlock);
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
