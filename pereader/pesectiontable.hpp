#pragma once

#include "pesection.hpp"

struct PESectionTable {
	std::vector<PESectionHeader> headers;
	std::vector<PESection> sections;

	PESectionTable ();

	static std::istream& Read (std::istream& stream, uint32_t numberOfSections, PESectionTable& sectionTable);

	template<class T>
	const T* GetInstanceOnVirtualAddress (uint64_t virtualAddress) const;
};

template <class T>
const T* PESectionTable::GetInstanceOnVirtualAddress (uint64_t virtualAddress) const {
	for (size_t i = 0, iEnd = headers.size (); i < iEnd; ++i) {
		const T* inst = sections[i].GetInstanceOnVirtualAddress<T> (headers[i], virtualAddress);
		if (inst) {
			return inst;
		}
	}

	return nullptr;
}
