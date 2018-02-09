#pragma once

#include "peheader.hpp"

class PE {
	std::string path;
	PEHeader header;

public:
	PE (const std::string& path);
	~PE ();

	bool Load ();

	const std::string& GetPath () const {
		return path;
	}

	const PEHeader& GetHeader () const {
		return header;
	}
};
