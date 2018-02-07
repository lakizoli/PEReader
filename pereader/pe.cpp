#include "stdafx.h"
#include "pe.hpp"

namespace fs = std::experimental::filesystem;

PE::PE (const std::string& path) :
	path (path)
{
}

PE::~PE () {
}

bool PE::Load () {
	fs::path pePath (path);
	if (!fs::exists (path)) {
		return false;
	}

	std::fstream fs (pePath.string (), std::ios::binary | std::ios::in);
	if (!fs) {
		return false;
	}

	//Read header
	fs >> header;
	if (!fs) {
		return false;
	}

	return true;
}
