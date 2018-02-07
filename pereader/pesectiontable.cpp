#include "stdafx.h"
#include "pesectiontable.hpp"

PESectionTable::PESectionTable () {
}

std::istream& PESectionTable::Read (std::istream& stream, uint32_t numberOfSections, PESectionTable& sectionTable) {
	//Reset content
	sectionTable.sectionHeaders.clear ();
	sectionTable.sections.clear ();

	//Read section headers
	for (uint32_t i = 0; i < numberOfSections; ++i) {
		PESectionHeader sectionHeader;
		stream >> sectionHeader;
		if (!stream) {
			return stream;
		}

		sectionTable.sectionHeaders.push_back (sectionHeader);
	}

	//Read secions
	for (uint32_t i = 0; i < numberOfSections; ++i) {
		const PESectionHeader& sectionHeader = sectionTable.sectionHeaders[i];

		PESection section;
		PESection::Read (stream, sectionHeader.PointerToRawData, sectionHeader.SizeOfRawData, section);
		if (!stream) {
			return stream;
		}

		sectionTable.sections.push_back (section);
	}

	return stream;
}
