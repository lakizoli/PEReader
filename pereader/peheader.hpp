#pragma once

#include "dosheader.hpp"
#include "pefileheader.hpp"
#include "peoptionalheader.hpp"

struct PEHeader {
	MSDOS20Header dosHeader;
	PEFileHeader fileHeader;
	PEOptionalHeader optionalHeader;

	PEHeader ();

	bool IsValid () const {
		return dosHeader.IsValid () && fileHeader.IsValid ();
	}
};

std::istream& operator >> (std::istream& stream, PEHeader& header);
