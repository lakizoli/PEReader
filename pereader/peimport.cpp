#include "stdafx.h"
#include "peimport.hpp"

PEImportDescriptor::PEImportDescriptor () {
	memset (this, 0, sizeof (*this));
}

PEBoundForwarderRef::PEBoundForwarderRef () {
	memset (this, 0, sizeof (*this));
}

PEBoundImportDescriptor::PEBoundImportDescriptor () {
	memset (this, 0, sizeof (*this));
}

PEDelayImportDescriptor::PEDelayImportDescriptor () {
	memset (this, 0, sizeof (*this));
}
