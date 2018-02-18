#pragma once

#include "dosheader.hpp"
#include "pefileheader.hpp"
#include "peoptionalheader.hpp"
#include "pesectiontable.hpp"
#include "peimport.hpp"

struct PEHeader {
	MSDOS20Header dosHeader;
	PEFileHeader fileHeader;
	PEOptionalHeader optionalHeader;
	PESectionTable sectionTable;

	PEHeader ();

	bool IsValid () const {
		return dosHeader.IsValid () && fileHeader.IsValid ();
	}

	typedef std::function<void (bool delayLoaded, const std::string& moduleName, const std::string& importFunction, uint64_t iatAddress)> ImportCallback;
	bool EnumerateImports (ImportCallback callback) const;

private:
	template<class T>
	void WalkImportFunctions (bool delayLoaded, uint32_t nameTableRVA, uint32_t iatTableRVA, const std::string& moduleName, ImportCallback callback) const;
};

template<class T>
void PEHeader::WalkImportFunctions (bool delayLoaded, uint32_t nameTableRVA, uint32_t iatTableRVA, const std::string& moduleName, ImportCallback callback) const {
	const T* importNameTable = sectionTable.GetInstanceOnVirtualAddress<T> (nameTableRVA);
	if (importNameTable) {
		uint32_t iatRVA = iatTableRVA;
		while (*importNameTable) {
			const uint8_t* importFunctionDescriptorAddress = sectionTable.GetInstanceOnVirtualAddress<uint8_t> (*importNameTable);
			if (importFunctionDescriptorAddress) {
				PEImportByName importByName = PEImportByName::Read (importFunctionDescriptorAddress);
				callback (delayLoaded, std::string (moduleName), importByName.Name, iatRVA);
			}

			iatRVA += sizeof (T);
			++importNameTable;
		}
	}
}

std::istream& operator >> (std::istream& stream, PEHeader& header);

