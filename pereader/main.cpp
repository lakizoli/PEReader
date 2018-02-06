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

	return 0;
}
