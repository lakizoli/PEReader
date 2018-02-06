#include "stdafx.h"
#include "dosheader.hpp"

MSDOS20Header::MSDOS20Header () {
	memset(this, 0, sizeof (*this));
	signature[0] = 'M';
	signature[1] = 'Z';
}

std::istream& operator >> (std::istream& stream, MSDOS20Header& header) {
	stream.read ((char*) &header, sizeof (header));
	return stream;
}
