#pragma once

struct PESection {
	std::vector<uint8_t> data;

	PESection ();

	static std::istream& Read (std::istream& stream, uint32_t offset, uint32_t size, PESection& section);
};

