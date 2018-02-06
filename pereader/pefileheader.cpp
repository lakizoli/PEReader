#include "stdafx.h"
#include "pefileheader.hpp"

PEFileHeader::PEFileHeader () {
	memset (this, 0, sizeof (*this));
	signature[0] = 'P';
	signature[1] = 'E';
}

std::istream& operator>> (std::istream& stream, PEFileHeader& header) {
	stream.read ((char*) &header, sizeof (header));
	return stream;
}
