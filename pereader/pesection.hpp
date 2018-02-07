#pragma once

#include "pesectionheader.hpp"

struct PESection {
	std::vector<uint8_t> data;

	PESection ();

	static std::istream& Read (std::istream& stream, uint32_t offset, uint32_t size, PESection& section);

	template<class T>
	const T* GetInstanceOnVirtualAddress (const PESectionHeader& header, uint64_t virtualAddress) const;
};

template <class T>
const T* PESection::GetInstanceOnVirtualAddress (const PESectionHeader& header, uint64_t virtualAddress) const {
	uint64_t startAddress = header.VirtualAddress;
	uint64_t endAddressExclusive = header.VirtualAddress + header.Misc.VirtualSize;

	if (virtualAddress >= startAddress && virtualAddress + sizeof (T) <= endAddressExclusive) {
		uint64_t offset = virtualAddress - header.VirtualAddress;
		if (data.size () >= offset + sizeof (T)) {
			return (const T*) &data[offset];
		}
	}

	return nullptr;
}
