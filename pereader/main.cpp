#include "stdafx.h"
#include "pe.hpp"
#include "utils.hpp"
#include "flatbinary.hpp"

static int PrintUsage (const char* exe) {
	std::cout <<
		"Usage:" << std::endl <<
		"    " << std::experimental::filesystem::path (exe).filename ().string () << " <command> <pe path> [destination path]" << std::endl <<
		std::endl <<
		"  commands:" << std::endl <<
		"    imports - dump all of PE file's imports" << std::endl <<
		"    exports - dump all of PE file's exports" << std::endl <<
		"    reloc   - dump all of PE file's relocations" << std::endl <<
		"    genbin  - generate flat binary file for execution to the given destination" << std::endl <<
		std::endl;

	return 1;
}

int main (int argc, char* argv[]) {
	if (argc < 3) {
		return PrintUsage (argc > 0 ? argv[0] : "PEReader");
	}

	PE pe (argv[2]);
	if (!pe.Load ()) {
		std::cout << "Error: cannot load PE file at path: " << argv[2] << std::endl << std::endl;
		return 1;
	}

	std::string command (argv[1]);
	if (command == "imports") { //show imports
		uint32_t cnt = 0;
		const PEHeader& header = pe.GetHeader ();
		if (!header.EnumerateImports ([&cnt] (bool delayLoaded, const std::string& moduleName, const std::string& importFunction, uint64_t iatAddress) -> void {
			std::string modulePath = Utils::GetFullPathOfModule (moduleName);

			std::string importType = delayLoaded ? "Delayed" : "Immediate";
			std::cout << importType << " import module: " << moduleName << ", function: " << importFunction << ", iatAddress: " << iatAddress << ", path: " << modulePath << std::endl;
			++cnt;
		})) {
			std::cout << "Error: cannot list imports defined in PE file at path: " << argv[2] << std::endl << std::endl;
			return 1;
		}
		std::cout << "Import count: " << std::dec << cnt << std::endl << std::endl;
	} else if (command == "exports") { //show exports
		uint32_t cnt = 0;
		const PEHeader& header = pe.GetHeader ();
		if (!header.EnumerateExports ([&cnt] (uint32_t ordinal, const std::string& name, uint64_t virtualAddress) -> void {
			std::cout << "Export function: " << name << ", ordinal: " << ordinal << ", virtualAddress: " << std::hex << virtualAddress << std::endl;
			++cnt;
		})) {
			std::cout << "Error: cannot list exports defined in PE file at path: " << argv[2] << std::endl << std::endl;
			return 1;
		}
		std::cout << "Export count: " << std::dec << cnt << std::endl << std::endl;
	} else if (command == "reloc") { //Show relocations
		uint32_t cnt = 0;
		const PEHeader& header = pe.GetHeader ();
		if (!header.EnumerateRelocations ([&cnt] (bool isHighPart, uint16_t wide, uint64_t virtualAddress) -> void {
			std::cout << "Relocation ishighPart: " << (isHighPart ? "true" : "false") << ", wide [byte]: " << wide << ", virtualAddress: " << std::hex << virtualAddress << std::endl;
			++cnt;
		})) {
			std::cout << "Error: cannot list relocations defined in PE file at path: " << argv[2] << std::endl << std::endl;
			return 1;
		}
		std::cout << "Relocation count: " << std::dec << cnt << std::endl << std::endl;
	} else if (command == "genbin") { //Generate binary for execution
		if (argc < 4) {
			std::cout << "Error: destination not given!" << std::endl << std::endl;
			return 1;
		}

		std::shared_ptr<FlatBinary> binary = FlatBinary::Create (pe);
		if (binary == nullptr) {
			std::cout << "Error: cannot create flat binary from file at path: " << argv[2] << std::endl << std::endl;
			return 1;
		}

		if (!binary->Save (argv[3])) {
			std::cout << "Error: cannot save flat binary to path: " << argv[3] << std::endl << std::endl;
			return 1;
		}
	} else { //Unknown/unhandled command
		return PrintUsage (argv[0]);
	}

	return 0;
}
