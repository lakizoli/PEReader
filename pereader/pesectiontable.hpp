#pragma once

struct PESectionHeader {
	std::string    Name;
	union {
		uint32_t   PhysicalAddress;
		uint32_t   VirtualSize;
	} Misc;
	uint32_t   VirtualAddress;
	uint32_t   SizeOfRawData;
	uint32_t   PointerToRawData;
	uint32_t   PointerToRelocations;
	uint32_t   PointerToLinenumbers;
	uint16_t   NumberOfRelocations;
	uint16_t   NumberOfLinenumbers;
	uint32_t   Characteristics;

	PESectionHeader ();
};

struct PESectionTable {
	std::vector<PESectionHeader> sections;

	PESectionTable ();
};

