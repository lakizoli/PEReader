#include "stdafx.h"
#include "pe.hpp"
#include "dosheader.hpp"
#include "pefileheader.hpp"

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <Windows.h>

namespace fs = std::experimental::filesystem;

PE::PE (const std::string& path) :
	mPath (path)
{
}

PE::~PE () {
}

bool PE::Load () {
	fs::path path (mPath);
	if (!fs::exists (path)) {
		return false;
	}

	std::fstream fs (path.string (), std::ios::binary | std::ios::in);
	if (!fs) {
		return false;
	}

	//Read header
	fs >> mHeader;
	if (!fs) {
		return false;
	}

	//TODO: ...

	return false;
}
