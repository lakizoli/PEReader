#pragma once

struct PEDataDirectory {
	uint32_t VirtualAddress;
	uint32_t Size;

	PEDataDirectory ();
};

std::istream& operator >> (std::istream& stream, PEDataDirectory& header);
