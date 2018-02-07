#include "stdafx.h"
#include "pe.hpp"

int main (int argc, char* argv[]) {
	if (argc <= 1) {
		//TODO: print usage...
		return 1;
	}

	PE pe (argv[1]);
	if (!pe.Load ()) {
		//TODO: print error
		return 1;
	}

	const PEHeader& header = pe.GetHeader ();
	if (!header.EnumerateImports ([] (bool delayLoaded, const std::string& moduleName, const std::string& importFunction) -> void {
		if (delayLoaded) {
			std::cout << "Delayed import module: " << moduleName << ", function: " << importFunction << std::endl;
		} else {
			std::cout << "Immediate import module: " << moduleName << ", function: " << importFunction << std::endl;
		}
		//TODO: ...
	})) {
		//TODO: print error
		return 1;
	}

	return 0;
}
