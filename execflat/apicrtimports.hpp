#pragma once

#include "importhandler.hpp"
#include "apicrtstate.hpp"

class ApiCrt_InitTerm : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_InitTerm);

	uint8_t* mFirstParam;
	uint8_t* mSecondParam;

	ApiCrt_InitTerm () : mFirstParam (nullptr), mSecondParam (nullptr) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_InitTermE : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_InitTermE);

	std::vector<uint8_t> mInjectBinary; ///< The binary code of the function to execute on the virtual processor.
	uint64_t mImportFunctionAddress;
	uint8_t* mFirstParam;
	uint8_t* mSecondParam;
	uint32_t mResultCode;

	ApiCrt_InitTermE () : mFirstParam (nullptr), mSecondParam (nullptr), mResultCode (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_SetAppType : public ImportHandlerWithState<ApiCrtState> {
	DECLARE_IMPORT_HANDLER (ApiCrt_SetAppType);

	uint32_t mAppType;

	ApiCrt_SetAppType () : ImportHandlerWithState ("ApiCrt"), mAppType (0) {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_SetFMode : public ImportHandlerWithState<ApiCrtState> {
	DECLARE_IMPORT_HANDLER (ApiCrt_SetFMode);

	uint32_t mFileMode;

	ApiCrt_SetFMode () : ImportHandlerWithState ("ApiCrt"), mFileMode (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_GetComMode : public ImportHandlerWithState<ApiCrtState> {
	DECLARE_IMPORT_HANDLER (ApiCrt_GetComMode);

	uint64_t mComModeRes;

	ApiCrt_GetComMode () : ImportHandlerWithState ("ApiCrt"), mComModeRes (0) {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};