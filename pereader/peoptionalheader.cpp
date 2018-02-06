#include "stdafx.h"
#include "peoptionalheader.hpp"

PEOptionalHeader32::PEOptionalHeader32 () {
	memset (this, 0, sizeof (*this));
}

PEOptionalHeaderROM::PEOptionalHeaderROM () {
	memset (this, 0, sizeof (*this));
}

PEOptionalHeader64::PEOptionalHeader64 () {
	memset (this, 0, sizeof (*this));
}

PEOptionalHeader::PEOptionalHeader () :
	type (HeaderTypes::IMAGE_UNKNOWN_OPTIONAL)
{
	header32 = nullptr;
	header64 = nullptr;
	headerROM = nullptr;
}

std::istream& operator>> (std::istream& stream, PEOptionalHeader& header) {
	//Reset content
	header.Data.clear ();
	header.DataDirectory.clear ();

	header.type = PEOptionalHeader::HeaderTypes::IMAGE_UNKNOWN_OPTIONAL;
	header.header32 = nullptr;
	header.header64 = nullptr;
	header.headerROM = nullptr;

	//Read magic
	header.Data.resize (sizeof (uint16_t));
	stream.read ((char*) &header.Data[0], sizeof (uint16_t));
	if (!stream) {
		return stream;
	}

	uint16_t magic = (uint16_t) header.Data[1] << 8 | (uint16_t) header.Data[0];

	//Read content
	size_t numberOfDirectories = 0;
	switch (magic) {
	case 0x10b: //32 bit optional header (PE32)
		header.type = PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR32_MAGIC;

		header.Data.resize (sizeof (PEOptionalHeader32));
		stream.read ((char*) &header.Data[2], sizeof (PEOptionalHeader32) - sizeof (uint16_t));
		if (!stream) {
			return stream;
		}

		header.header32 = (PEOptionalHeader32*) &header.Data[0];
		numberOfDirectories = header.header32->NumberOfRvaAndSizes;
		break;
	case 0x20b: //64 bit optional header (PE32+)
		header.type = PEOptionalHeader::HeaderTypes::IMAGE_NT_OPTIONAL_HDR64_MAGIC;

		header.Data.resize (sizeof (PEOptionalHeader64));
		stream.read ((char*) &header.Data[2], sizeof (PEOptionalHeader64) - sizeof (uint16_t));
		if (!stream) {
			return stream;
		}

		header.header64 = (PEOptionalHeader64*) &header.Data[0];
		numberOfDirectories = header.header64->NumberOfRvaAndSizes;
		break;
	case 0x107: //Rom optional header
		header.type = PEOptionalHeader::HeaderTypes::IMAGE_ROM_OPTIONAL_HDR_MAGIC;

		header.Data.resize (sizeof (PEOptionalHeaderROM));
		stream.read ((char*) &header.Data[2], sizeof (PEOptionalHeaderROM) - sizeof (uint16_t));
		if (!stream) {
			return stream;
		}

		header.headerROM = (PEOptionalHeaderROM*) &header.Data[0];
		break;
	default: //Unknown optional header format
		stream.setstate (std::ios_base::failbit);
		return stream;
	}

	//Read directories
	for (size_t i = 0; i < numberOfDirectories; ++i) {
		PEDataDirectory dir;
		stream >> dir;
		if (!stream) {
			return stream;
		}

		header.DataDirectory.push_back (dir);
	}

	return stream;
}
