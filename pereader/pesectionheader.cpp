#include "stdafx.h"
#include "pesectionheader.hpp"

PESectionHeader::PESectionHeader () {
	memset (this, 0, sizeof (*this));
}

std::istream& operator>> (std::istream& stream, PESectionHeader& header) {
	stream.read ((char*) &header, sizeof (header));
	return stream;
}
