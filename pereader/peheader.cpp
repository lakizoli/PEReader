#include "stdafx.h"
#include "peheader.hpp"
#include "peoptionalheader.hpp"

PEHeader::PEHeader () {
}

std::istream& operator>> (std::istream& stream, PEHeader& header) {
	//Read DOS header
	stream >> header.dosHeader;
	if (!stream) {
		return stream;
	}

	if (!header.dosHeader.IsValid ()) {
		stream.setstate (std::ios_base::failbit);
		return stream;
	}

	//Seek to PE file header
	stream.seekg (header.dosHeader.e_lfanew, std::ios_base::beg);
	if (!stream) {
		return stream;
	}

	//Read PE file header
	stream >> header.fileHeader;
	if (!stream) {
		return stream;
	}

	if (!header.fileHeader.IsValid ()) {
		stream.setstate (std::ios_base::failbit);
		return stream;
	}

	//Read optional header
	stream >> header.optionalHeader;
	if (!stream) {
		return stream;
	}

	//TODO: ...

	return stream;
}
