#pragma once

struct MSDOS20Header {
	uint8_t signature[2]; // = { 'M', 'Z' };
	uint16_t lastsize;
	uint16_t nblocks;
	uint16_t nreloc;
	uint16_t hdrsize;
	uint16_t minalloc;
	uint16_t maxalloc;
	uint16_t ss; // void* - 2 byte value
	uint16_t sp; // void*- 2 byte value
	uint16_t checksum;
	uint16_t ip; // void* - 2 byte value
	uint16_t cs; // void* - 2 byte value
	uint16_t relocpos;
	uint16_t noverlay;
	uint16_t reserved1[4];
	uint16_t oem_id;
	uint16_t oem_info;
	uint16_t reserved2[10];
	uint32_t e_lfanew; // Offset to the 'PE\0\0' signature relative to the beginning of the file

	MSDOS20Header ();

	bool IsValid () const {
		return signature[0] == 'M' && signature[1] == 'Z';
	}
};

std::istream& operator >> (std::istream& stream, MSDOS20Header& header);
