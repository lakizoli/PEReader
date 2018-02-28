#pragma once

#include "flatbinary.hpp"

class Analyzer {
	std::string mFlatPath;
	std::string mDestPath;

public:
	Analyzer (const std::string& flatPath, const std::string& destPath);
	~Analyzer ();

	void Execute (std::shared_ptr<FlatBinary> binary);
};

