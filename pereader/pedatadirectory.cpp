#include "stdafx.h"
#include "pedatadirectory.hpp"

PEDataDirectory::PEDataDirectory () :
	VirtualAddress(0),
	Size(0)
{
}

std::istream& operator>> (std::istream& stream, PEDataDirectory& header) {
	stream.read ((char*) &header, sizeof (header));
	return stream;
}
