#include "memory.h"

Bit64u BX_MEM_C::get_memory_len (void) {
	//TODO: ...
	return 0;
}

Bit8u* BX_MEM_C::getHostMemAddr (BX_CPU_C* cpu, bx_phy_address addr, unsigned rw) {
	//TODO: ...
	return nullptr;
}

void BX_MEM_C::readPhysicalPage (BX_CPU_C* cpu, bx_phy_address addr, unsigned len, void* data) {
	//TODO: ...
}

void BX_MEM_C::writePhysicalPage (BX_CPU_C* cpu, bx_phy_address addr, unsigned len, void* data) {
	//TODO: ...
}
