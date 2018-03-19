#include "stdafx.h"
#include "importcpuhook.hpp"
#include "importhandler.hpp"
#include <flatbinary.hpp>

std::string ImportCpuHook::FindImport (uint64_t virtualAddress) const {
	const std::map<uint64_t, std::string>& imports = mBinary->GetImports ();
	const uint64_t importAddress = virtualAddress - mLoadBase;
	auto it = imports.find (importAddress);
	return it != imports.end () ? it->second : std::string ();
}

BX_CPU_C::ICalledImport* ImportCpuHook::RunImport (bool isCall, Bit64u address) const {
	std::string importDeclaration = FindImport (address);
	if (importDeclaration.empty ()) {
		return nullptr;
	}

	std::cout << (isCall ? "Calling" : "Jump to") << " import: " << importDeclaration << std::endl;

	std::shared_ptr<ImportHandler> importHandler = ImportHandler::Create (importDeclaration);
	if (importHandler == nullptr) {
		return nullptr;
	}

	importHandler->ReadParameters (mCpu, mInjectBase);
	importHandler->Call ();

	return new CalledImport (importHandler);
}

ImportCpuHook::ImportCpuHook (BX_CPU_C& cpu, uint64_t loadBase, uint64_t injectBase, std::shared_ptr<FlatBinary> binary) :
	mCpu (cpu),
	mLoadBase (loadBase),
	mInjectBase (injectBase),
	mBinary (binary) {
}

bool ImportCpuHook::IsImport (Bit64u address) const {
	const std::map<uint64_t, std::string>& imports = mBinary->GetImports ();
	const uint64_t importAddress = address - mLoadBase;
	return imports.find (importAddress) != imports.end ();
}

bool ImportCpuHook::CloseImportCall (BX_CPU_C::ICalledImport* calledImport) {
	if (calledImport == nullptr) {
		return false;
	}

	CalledImport* calledImportInst = (CalledImport*) calledImport;
	std::shared_ptr<ImportHandler> importHandler = calledImportInst->importHandler;
	delete calledImportInst;

	return importHandler->WriteResults (mCpu);
}
