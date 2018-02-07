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

PEImportByName::PEImportByName () :
	Hint (0)
{
}

PEImportByName PEImportByName::Read (const uint8_t* address) {
	PEImportByName res;

	res.Hint = (uint16_t) address[1] << 8 | (uint16_t) address[0];
	res.Name = (const char*) (address + 2);

	return res;
}
