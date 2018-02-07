#pragma once

#include "pesectionheader.hpp"
#include "pesection.hpp"

struct PESectionTable {
	std::vector<PESectionHeader> sectionHeaders;
	std::vector<PESection> sections;

	PESectionTable ();

	static std::istream& Read (std::istream& stream, uint32_t numberOfSections, PESectionTable& sectionTable);
};

