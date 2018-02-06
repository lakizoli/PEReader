#pragma once

#include "peheader.hpp"

class PE {
	std::string mPath;
	PEHeader mHeader;

public:
	PE (const std::string& path);
	~PE ();

	bool Load ();
};
