#pragma once

#include "importhandler.hpp"
#include <Windows.h>

class Kernel32_GetSystemTimeAsFileTime : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_GetSystemTimeAsFileTime);

	uint64_t mResultVirtualAddress;
	FILETIME mCallResult;

	Kernel32_GetSystemTimeAsFileTime () : mResultVirtualAddress (0), mCallResult { 0,0 } {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu) override;
};

class Kernel32_GetCurrentThreadId : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_GetCurrentThreadId);

	DWORD mThreadId;

	Kernel32_GetCurrentThreadId () : mThreadId (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu) override;
};

class Kernel32_GetCurrentProcessId : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_GetCurrentProcessId);

	DWORD mProcessId;

	Kernel32_GetCurrentProcessId () : mProcessId (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu) override;
};

class Kernel32_QueryPerformanceCounter : public ImportHandler {
	DECLARE_IMPORT_HANDLER (Kernel32_QueryPerformanceCounter);

	uint64_t mCounterVirtualAddress;
	LARGE_INTEGER mCounter;
	BOOL mReturnValue;

	Kernel32_QueryPerformanceCounter () {}

public:
	void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu) override;
};
