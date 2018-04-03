#pragma once

#include "importstate.hpp"

struct Kernel32State : public ImportStateItem {
	DECLARE_IMPORT_STATE_ITEM (Kernel32State);

	//HMODULE stub for GetModuleHandleW
	uint8_t moduleHeader[0x1000];

public:
	Kernel32State ();
};

