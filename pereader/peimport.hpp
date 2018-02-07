#pragma once

struct PEImportDescriptor {
	union {
		uint32_t Characteristics;			// 0 for terminating null import descriptor
		uint32_t OriginalFirstThunk;		// RVA to original unbound IAT (PIMAGE_THUNK_DATA)
	};
	uint32_t TimeDateStamp;					// 0 if not bound,
											// -1 if bound, and real date\time stamp
											//     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
											// O.W. date/time stamp of DLL bound to (Old BIND)

	uint32_t ForwarderChain;				// -1 if no forwarders
	uint32_t Name;
	uint32_t FirstThunk;					// RVA to IAT (if bound this IAT has actual addresses)

	PEImportDescriptor ();
};

struct PEBoundForwarderRef {
	uint32_t TimeDateStamp;
	uint16_t OffsetModuleName;
	uint16_t Reserved;

	PEBoundForwarderRef ();
};

struct PEBoundImportDescriptor {
	uint32_t TimeDateStamp;
	uint16_t OffsetModuleName;
	uint16_t NumberOfModuleForwarderRefs;
	PEBoundForwarderRef* ModuleForwarderRefs; // Array of zero or more (NumberOfModuleForwarderRefs) PEBoundForwarderRef

	PEBoundImportDescriptor ();
};

struct PEDelayImportDescriptor {
	union {
		uint32_t AllAttributes;
		struct {
			uint32_t RvaBased : 1;				// Delay load version 2
			uint32_t ReservedAttributes : 31;
		};
	} Attributes;

	uint32_t DllNameRVA;						// RVA to the name of the target library (NULL-terminate ASCII string)
	uint32_t ModuleHandleRVA;					// RVA to the HMODULE caching location (PHMODULE)
	uint32_t ImportAddressTableRVA;				// RVA to the start of the IAT (PIMAGE_THUNK_DATA)
	uint32_t ImportNameTableRVA;				// RVA to the start of the name table (PIMAGE_THUNK_DATA::AddressOfData)
	uint32_t BoundImportAddressTableRVA;		// RVA to an optional bound IAT
	uint32_t UnloadInformationTableRVA;			// RVA to an optional unload info table
	uint32_t TimeDateStamp;						// 0 if not bound,
												// Otherwise, date/time of the target DLL

	PEDelayImportDescriptor ();
};

struct PEImportByName {
	uint16_t Hint;
	std::string Name;

	PEImportByName ();

	static PEImportByName Read (const uint8_t* address);
};
