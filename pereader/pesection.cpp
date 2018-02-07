#include "stdafx.h"
#include "pesection.hpp"

PESection::PESection () {
}

std::istream& PESection::Read (std::istream& stream, uint32_t offset, uint32_t size, PESection& section) {
	if (size > 0) {
		stream.seekg (offset, std::ios::beg);
		if (!stream) {
			return stream;
		}

		section.data.resize (size);
		stream.read ((char*) &section.data[0], size);
		if (!stream) {
			return stream;
		}
	}

	return stream;
}
