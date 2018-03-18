#include "stdafx.h"
#include "perelocation.hpp"

PEBaseRelocation::PEBaseRelocation () {
	memset (this, 0, sizeof (*this));
}

PEBaseRelocationValue::PEBaseRelocationValue () {
	memset (this, 0, sizeof (*this));
}
