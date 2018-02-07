#include "stdafx.h"
#include "pesectiontable.hpp"

PESectionTable::PESectionTable () {
}

std::istream& PESectionTable::Read (std::istream& stream, uint32_t numberOfSections, PESectionTable& sectionTable) {
	//Reset content
	sectionTable.headers.clear ();
	sectionTable.sections.clear ();

	//Read section headers
	for (uint32_t i = 0; i < numberOfSections; ++i) {
		PESectionHeader header;
		stream >> header;
		if (!stream) {
			return stream;
		}

		sectionTable.headers.push_back (header);
	}

	//Read secions
	for (uint32_t i = 0; i < numberOfSections; ++i) {
		const PESectionHeader& header = sectionTable.headers[i];

		PESection section;
		PESection::Read (stream, header.PointerToRawData, header.SizeOfRawData, section);
		if (!stream) {
			return stream;
		}

		sectionTable.sections.push_back (section);
	}

	return stream;
}
