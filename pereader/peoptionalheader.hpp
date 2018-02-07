#pragma once

struct PEDataDirectory {
	uint32_t VirtualAddress;
	uint32_t Size;

	PEDataDirectory ();
};

std::istream& operator >> (std::istream& stream, PEDataDirectory& header);

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
	enum class HeaderTypes : uint16_t {
		IMAGE_UNKNOWN_OPTIONAL = 0,

		IMAGE_NT_OPTIONAL_HDR32_MAGIC = 0x10b, //32 bit executable image.
		IMAGE_NT_OPTIONAL_HDR64_MAGIC = 0x20b, //64 bit executable image
		IMAGE_ROM_OPTIONAL_HDR_MAGIC  = 0x107, //ROM image
	};

	enum class Subsystems : uint16_t {
		IMAGE_SUBSYSTEM_UNKNOWN						= 0, // Unknown subsystem.
		IMAGE_SUBSYSTEM_NATIVE						= 1, // Image doesn't require a subsystem.
		IMAGE_SUBSYSTEM_WINDOWS_GUI					= 2, // Image runs in the Windows GUI subsystem.
		IMAGE_SUBSYSTEM_WINDOWS_CUI					= 3, // Image runs in the Windows character subsystem.
		IMAGE_SUBSYSTEM_OS2_CUI						= 5, // image runs in the OS/2 character subsystem.
		IMAGE_SUBSYSTEM_POSIX_CUI					= 7, // image runs in the Posix character subsystem.
		IMAGE_SUBSYSTEM_NATIVE_WINDOWS				= 8, // image is a native Win9x driver.
		IMAGE_SUBSYSTEM_WINDOWS_CE_GUI				= 9, // Image runs in the Windows CE subsystem.
		IMAGE_SUBSYSTEM_EFI_APPLICATION				= 10, //
		IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER		= 11,  //
		IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER			= 12, //
		IMAGE_SUBSYSTEM_EFI_ROM						= 13,
		IMAGE_SUBSYSTEM_XBOX						= 14,
		IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION	= 16,
		IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG			= 17
	};

	enum class DLLCharacteristics : uint16_t {
		IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA		= 0x0020,  // Image can handle a high entropy 64-bit virtual address space.
		IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE			= 0x0040,  // DLL can move.
		IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY		= 0x0080,  // Code Integrity Image
		IMAGE_DLLCHARACTERISTICS_NX_COMPAT				= 0x0100,  // Image is NX compatible
		IMAGE_DLLCHARACTERISTICS_NO_ISOLATION			= 0x0200,  // Image understands isolation and doesn't want it
		IMAGE_DLLCHARACTERISTICS_NO_SEH					= 0x0400,  // Image does not use SEH.  No SE handler may reside in this image
		IMAGE_DLLCHARACTERISTICS_NO_BIND				= 0x0800,  // Do not bind this image.
		IMAGE_DLLCHARACTERISTICS_APPCONTAINER			= 0x1000,  // Image should execute in an AppContainer
		IMAGE_DLLCHARACTERISTICS_WDM_DRIVER				= 0x2000,  // Driver uses WDM model
		IMAGE_DLLCHARACTERISTICS_GUARD_CF				= 0x4000,  // Image supports Control Flow Guard.
		IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE	= 0x8000
	};

	enum class DirectoryEntries : uint16_t {
		IMAGE_DIRECTORY_ENTRY_EXPORT			= 0,  // Export Directory
		IMAGE_DIRECTORY_ENTRY_IMPORT			= 1,  // Import Directory
		IMAGE_DIRECTORY_ENTRY_RESOURCE			= 2,  // Resource Directory
		IMAGE_DIRECTORY_ENTRY_EXCEPTION			= 3,  // Exception Directory
		IMAGE_DIRECTORY_ENTRY_SECURITY			= 4,  // Security Directory
		IMAGE_DIRECTORY_ENTRY_BASERELOC			= 5,  // Base Relocation Table
		IMAGE_DIRECTORY_ENTRY_DEBUG				= 6,  // Debug Directory
		IMAGE_DIRECTORY_ENTRY_ARCHITECTURE		= 7,  // Architecture Specific Data
		IMAGE_DIRECTORY_ENTRY_GLOBALPTR			= 8,  // RVA of GP
		IMAGE_DIRECTORY_ENTRY_TLS				= 9,  // TLS Directory
		IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG		= 10,  // Load Configuration Directory
		IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT		= 11,  // Bound Import Directory in headers
		IMAGE_DIRECTORY_ENTRY_IAT				= 12,  // Import Address Table
		IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT		= 13,  // Delay Load Import Descriptors
		IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR	= 14   // COM Runtime descriptor
	};

	std::vector<uint8_t> data;
	std::vector<PEDataDirectory> dataDirectories;

	HeaderTypes type;
	union {
		PEOptionalHeader32* header32;
		PEOptionalHeader64* header64;
		PEOptionalHeaderROM* headerROM;
	};

	PEOptionalHeader ();
};

std::istream& operator >> (std::istream& stream, PEOptionalHeader& header);
