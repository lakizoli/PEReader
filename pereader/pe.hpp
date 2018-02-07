#pragma once

#include "peheader.hpp"

class PE {
	std::string path;
	PEHeader header;

public:
	PE (const std::string& path);
	~PE ();

	bool Load ();

	const PEHeader& GetHeader () const {
		return header;
	}
};
