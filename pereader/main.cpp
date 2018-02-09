#include "stdafx.h"
#include "pe.hpp"
#include "utils.hpp"

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

	uint32_t cnt = 0;
	const PEHeader& header = pe.GetHeader ();
	if (!header.EnumerateImports ([&cnt] (bool delayLoaded, const std::string& moduleName, const std::string& importFunction) -> void {
		std::string modulePath = Utils::GetFullPathOfModule (moduleName);

		std::string importType = delayLoaded ? "Delayed" : "Immediate";
		std::cout << importType << " import module: " << moduleName << ", function: " << importFunction << ", path: " << modulePath << std::endl;
		++cnt;
	})) {
		//TODO: print error
		return 1;
	}
	std::cout << "Import count: " << cnt << std::endl << std::endl;

	return 0;
}
