#pragma once

#include "importhandler.hpp"
#include "kernel32state.hpp"
#include <Windows.h>

class Kernel32_GetSystemTimeAsFileTime : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_GetSystemTimeAsFileTime);

	uint64_t mResultVirtualAddress;
	FILETIME mCallResult;

	Kernel32_GetSystemTimeAsFileTime () : mResultVirtualAddress (0), mCallResult{ 0,0 } {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class Kernel32_GetCurrentThreadId : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_GetCurrentThreadId);

	DWORD mThreadId;

	Kernel32_GetCurrentThreadId () : mThreadId (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class Kernel32_GetCurrentProcessId : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_GetCurrentProcessId);

	DWORD mProcessId;

	Kernel32_GetCurrentProcessId () : mProcessId (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class Kernel32_QueryPerformanceCounter : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_QueryPerformanceCounter);

	uint64_t mCounterVirtualAddress;
	LARGE_INTEGER mCounter;
	BOOL mReturnValue;

	Kernel32_QueryPerformanceCounter () {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class Kernel32_InitializeSListHead : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_InitializeSListHead);

	uint8_t* mHead;

	Kernel32_InitializeSListHead () : mHead (nullptr) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class Kernel32_SetUnhandledExceptionFilter : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_SetUnhandledExceptionFilter);

	uint8_t* mTopLevelExceptionFilter;
	uint64_t mLastTopLevelExceptionFilter;

	Kernel32_SetUnhandledExceptionFilter () : mTopLevelExceptionFilter (nullptr), mLastTopLevelExceptionFilter (0) {}

public:
	void ReadParameters(BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call() override;
	bool WriteResults(BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};

class Kernel32_GetModuleHandleW : public ImportHandlerWithState<Kernel32State> {
	DECLARE_IMPORT_HANDLER (Kernel32_GetModuleHandleW);

	uint8_t* mModuleName;
	HMODULE mHandle;
	uint64_t mVirtualHandle;

	Kernel32_GetModuleHandleW () : ImportHandlerWithState ("Kernel32"), mModuleName (nullptr), mHandle (0), mVirtualHandle (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) override;
};