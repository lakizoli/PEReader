#pragma once

#include "pedatadirectory.hpp"

struct PEOptionalHeader32 {
	//
	// Standard fields.
	//

	uint16_t   Magic;
	uint8_t    MajorLinkerVersion;
	uint8_t    MinorLinkerVersion;
	uint32_t   SizeOfCode;
	uint32_t   SizeOfInitializedData;
	uint32_t   SizeOfUninitializedData;
	uint32_t   AddressOfEntryPoint;
	uint32_t   BaseOfCode;
	uint32_t   BaseOfData;

	//
	// NT additional fields.
	//

	uint32_t   ImageBase;
	uint32_t   SectionAlignment;
	uint32_t   FileAlignment;
	uint16_t   MajorOperatingSystemVersion;
	uint16_t   MinorOperatingSystemVersion;
	uint16_t   MajorImageVersion;
	uint16_t   MinorImageVersion;
	uint16_t   MajorSubsystemVersion;
	uint16_t   MinorSubsystemVersion;
	uint32_t   Win32VersionValue;
	uint32_t   SizeOfImage;
	uint32_t   SizeOfHeaders;
	uint32_t   CheckSum;
	uint16_t   Subsystem;
	uint16_t   DllCharacteristics;
	uint32_t   SizeOfStackReserve;
	uint32_t   SizeOfStackCommit;
	uint32_t   SizeOfHeapReserve;
	uint32_t   SizeOfHeapCommit;
	uint32_t   LoaderFlags;
	uint32_t   NumberOfRvaAndSizes;

	PEOptionalHeader32 ();
};

struct PEOptionalHeaderROM {
	uint16_t  Magic;
	uint8_t   MajorLinkerVersion;
	uint8_t   MinorLinkerVersion;
	uint32_t  SizeOfCode;
	uint32_t  SizeOfInitializedData;
	uint32_t  SizeOfUninitializedData;
	uint32_t  AddressOfEntryPoint;
	uint32_t  BaseOfCode;
	uint32_t  BaseOfData;
	uint32_t  BaseOfBss;
	uint32_t  GprMask;
	uint32_t  CprMask[4];
	uint32_t  GpValue;

	PEOptionalHeaderROM ();
};

struct PEOptionalHeader64 {
	uint16_t   Magic;
	uint8_t    MajorLinkerVersion;
	uint8_t    MinorLinkerVersion;
	uint32_t   SizeOfCode;
	uint32_t   SizeOfInitializedData;
	uint32_t   SizeOfUninitializedData;
	uint32_t   AddressOfEntryPoint;
	uint32_t   BaseOfCode;
	uint64_t   ImageBase;
	uint32_t   SectionAlignment;
	uint32_t   FileAlignment;
	uint16_t   MajorOperatingSystemVersion;
	uint16_t   MinorOperatingSystemVersion;
	uint16_t   MajorImageVersion;
	uint16_t   MinorImageVersion;
	uint16_t   MajorSubsystemVersion;
	uint16_t   MinorSubsystemVersion;
	uint32_t   Win32VersionValue;
	uint32_t   SizeOfImage;
	uint32_t   SizeOfHeaders;
	uint32_t   CheckSum;
	uint16_t   Subsystem;
	uint16_t   DllCharacteristics;
	uint64_t   SizeOfStackReserve;
	uint64_t   SizeOfStackCommit;
	uint64_t   SizeOfHeapReserve;
	uint64_t   SizeOfHeapCommit;
	uint32_t   LoaderFlags;
	uint32_t   NumberOfRvaAndSizes;

	PEOptionalHeader64 ();
};

struct PEOptionalHeader {
	enum class HeaderTypes {
		IMAGE_UNKNOWN_OPTIONAL = 0,

		IMAGE_NT_OPTIONAL_HDR32_MAGIC = 0x10b, //32 bit executable image.
		IMAGE_NT_OPTIONAL_HDR64_MAGIC = 0x20b, //64 bit executable image
		IMAGE_ROM_OPTIONAL_HDR_MAGIC  = 0x107, //ROM image
	};

	std::vector<uint8_t> Data;
	std::vector<PEDataDirectory> DataDirectory;

	HeaderTypes type;
	union {
		PEOptionalHeader32* header32;
		PEOptionalHeader64* header64;
		PEOptionalHeaderROM* headerROM;
	};

	PEOptionalHeader ();
};

std::istream& operator >> (std::istream& stream, PEOptionalHeader& header);
