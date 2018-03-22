#pragma once

#include <x86cpu.h>

class FlatBinary;
class ImportHandler;
class ImportState;

class ImportCpuHook : public BX_CPU_C::IImportHook {
	struct CalledImport : public BX_CPU_C::ICalledImport {
		std::shared_ptr<ImportHandler> importHandler;

		CalledImport (std::shared_ptr<ImportHandler> importHandler) : importHandler (importHandler) {}
	};

	BX_CPU_C& mCpu;
	uint64_t mLoadBase;
	uint64_t mInjectBase;
	uint64_t mStateBase;
	std::shared_ptr<FlatBinary> mBinary;
	std::shared_ptr<ImportState> mState;

	std::string FindImport (uint64_t virtualAddress) const;
	BX_CPU_C::ICalledImport* RunImport (bool isCall, Bit64u address) const;

public:
	ImportCpuHook (BX_CPU_C& cpu, uint64_t loadBase, uint64_t injectBase, uint64_t stateBase, std::shared_ptr<FlatBinary> binary);

	virtual bool IsImport (Bit64u address) const override;

	virtual BX_CPU_C::ICalledImport* CallImport (Bit64u address) override {
		return RunImport (true, address);
	}

	virtual BX_CPU_C::ICalledImport* JumpImport (Bit64u address) override {
		return RunImport (false, address);
	}

	virtual bool CloseImportCall (BX_CPU_C::ICalledImport* calledImport) override;
};
