#pragma once

#include "importhandler.hpp"
#include "apicrtstate.hpp"

//[api-ms-win-crt-runtime-l1-1-0.dll]

class ApiCrt_InitTerm : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_InitTerm);

	std::vector<uint8_t> mInjectBinary; ///< The binary code of the function to execute on the virtual processor.
	uint64_t mImportFunctionAddress;

	ApiCrt_InitTerm () {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_InitTermE : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_InitTermE);

	std::vector<uint8_t> mInjectBinary; ///< The binary code of the function to execute on the virtual processor.
	uint64_t mImportFunctionAddress;

	ApiCrt_InitTermE () {}

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
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_AtExit : public ImportHandlerWithState<ApiCrtState> {
	DECLARE_IMPORT_HANDLER (ApiCrt_AtExit);

	uint64_t mFuncAddress;
	uint32_t mResult;

	ApiCrt_AtExit () : ImportHandlerWithState ("ApiCrt"), mFuncAddress (0), mResult (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_ConfigureNarrowArgv : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_ConfigureNarrowArgv);

	ApiCrt_ConfigureNarrowArgv () {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_InitializeNarrowEnvironment : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_InitializeNarrowEnvironment);

	ApiCrt_InitializeNarrowEnvironment () {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_GetArgv : public ImportHandlerWithState<ApiCrtState> {
	DECLARE_IMPORT_HANDLER (ApiCrt_GetArgv);

	uint64_t mArgvVirtualAddress;

	ApiCrt_GetArgv () : ImportHandlerWithState ("ApiCrt") {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_GetArgc : public ImportHandlerWithState<ApiCrtState> {
	DECLARE_IMPORT_HANDLER (ApiCrt_GetArgc);

	uint64_t mArgcVirtualAddress;

	ApiCrt_GetArgc () : ImportHandlerWithState ("ApiCrt") {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_GetInitialNarrowEnvironment : public ImportHandlerWithState<ApiCrtState> {
	DECLARE_IMPORT_HANDLER (ApiCrt_GetInitialNarrowEnvironment);

	uint64_t mInitEnv;

	ApiCrt_GetInitialNarrowEnvironment () : ImportHandlerWithState ("ApiCrt") {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

//[api-ms-win-crt-stdio-l1-1-0.dll]

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
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_IobFunc : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_IobFunc);

	uint32_t mIobIndex;
	uint64_t mIobFunc;

	ApiCrt_IobFunc () : mIobIndex (0), mIobFunc (0) {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class ApiCrt_CommonVFPrintf : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_CommonVFPrintf);

	uint64_t mOptions;
	uint64_t mFileParam;
	const char* mFormatParam;
	uint8_t* mLocale;
	va_list mFormatParamList;
	int32_t mPrintfResult;

	ApiCrt_CommonVFPrintf () : mOptions (0), mFileParam (0), mFormatParam (nullptr), mLocale (nullptr), mFormatParamList (0), mPrintfResult (0) {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

//[api-ms-win-crt-locale-l1-1-0.dll]

class ApiCrt_ConfigThreadLocale : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_ConfigThreadLocale);

	int32_t mType;
	int32_t mResult;

	ApiCrt_ConfigThreadLocale () : mType (0), mResult (0) {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

//[api-ms-win-crt-heap-l1-1-0.dll]

class ApiCrt_SetNewMode : public ImportHandlerWithState<ApiCrtState> {
	DECLARE_IMPORT_HANDLER (ApiCrt_SetNewMode);

	int32_t mMode;
	int32_t mOldMode;

	ApiCrt_SetNewMode () : ImportHandlerWithState ("ApiCrt"), mMode (0), mOldMode (0) {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};
