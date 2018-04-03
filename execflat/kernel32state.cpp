#include "stdafx.h"
#include "kernel32state.hpp"

IMPLEMENT_IMPORT_STATE_ITEM (Kernel32State, "Kernel32");

Kernel32State::Kernel32State () {
	memset (moduleHeader, 0, sizeof (moduleHeader));
}
