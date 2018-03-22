#include "stdafx.h"
#include "importhandler.hpp"

std::map<std::string, ImportHandler::creator>& ImportHandler::GetCreatorMap () {
	static std::map<std::string, creator> creatorMap;
	return creatorMap;
}

uint32_t ImportHandler::RegisterCreator (const std::string& importDeclaration, creator creator) {
	GetCreatorMap ().emplace (importDeclaration, creator);
	return 42;
}

std::shared_ptr<ImportHandler> ImportHandler::Create (const std::string& importDeclaration) {
	const auto& creatorMap = GetCreatorMap ();
	auto it = creatorMap.find (importDeclaration);
	if (it == creatorMap.end ()) {
		return nullptr;
	}
	return it->second ();
}

uint8_t* ImportHandler::GetAddressOfVirtualAddress (BX_CPU_C& cpu, uint64_t virtualAddress) {
	if (virtualAddress == 0) {
		return nullptr;
	}

	//TODO: Have to handle multiple memory pages!
	return cpu.mem->get_vector (&cpu, virtualAddress);
}
