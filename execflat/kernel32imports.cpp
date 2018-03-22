#include "stdafx.h"
#include "kernel32imports.hpp"

IMPLEMENT_IMPORT_HANDLER (Kernel32_GetSystemTimeAsFileTime, "[KERNEL32.dll]GetSystemTimeAsFileTime");
IMPLEMENT_IMPORT_HANDLER (Kernel32_GetCurrentThreadId, "[KERNEL32.dll]GetCurrentThreadId");
IMPLEMENT_IMPORT_HANDLER (Kernel32_GetCurrentProcessId, "[KERNEL32.dll]GetCurrentProcessId");
IMPLEMENT_IMPORT_HANDLER (Kernel32_QueryPerformanceCounter, "[KERNEL32.dll]QueryPerformanceCounter");
IMPLEMENT_IMPORT_HANDLER (Kernel32_InitializeSListHead, "[KERNEL32.dll]InitializeSListHead");
IMPLEMENT_IMPORT_HANDLER (Kernel32_SetUnhandledExceptionFilter, "[KERNEL32.dll]SetUnhandledExceptionFilter");



void Kernel32_GetSystemTimeAsFileTime::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mResultVirtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
}

void Kernel32_GetSystemTimeAsFileTime::Call () {
	GetSystemTimeAsFileTime (&mCallResult);
}

bool Kernel32_GetSystemTimeAsFileTime::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	return CopyToVirtualMemory (cpu, mCallResult, mResultVirtualAddress, sizeof (FILETIME));
}



void Kernel32_GetCurrentThreadId::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	//No parameters needed
}

void Kernel32_GetCurrentThreadId::Call () {
	mThreadId = GetCurrentThreadId ();
}

bool Kernel32_GetCurrentThreadId::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	return WriteValueToGPRegister (cpu, (uint32_t) mThreadId, BX_64BIT_REG_RAX);
}



void Kernel32_GetCurrentProcessId::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	//No parameters needed
}

void Kernel32_GetCurrentProcessId::Call () {
	mProcessId = GetCurrentProcessId ();
}

bool Kernel32_GetCurrentProcessId::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	return WriteValueToGPRegister (cpu, (uint32_t) mProcessId, BX_64BIT_REG_RAX);
}



void Kernel32_QueryPerformanceCounter::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mCounterVirtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
}

void Kernel32_QueryPerformanceCounter::Call () {
	mReturnValue = QueryPerformanceCounter (&mCounter);
}

bool Kernel32_QueryPerformanceCounter::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	if (!CopyToVirtualMemory (cpu, mCounter, mCounterVirtualAddress, sizeof (LARGE_INTEGER))) {
		return false;
	}

	return WriteValueToGPRegister (cpu, (uint32_t) mReturnValue, BX_64BIT_REG_RAX);
}



void Kernel32_InitializeSListHead::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	uint64_t virtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
	mHead = GetAddressOfVirtualAddress (cpu, virtualAddress);
}

void Kernel32_InitializeSListHead::Call () {
	InitializeSListHead ((PSLIST_HEADER) mHead);
}

bool Kernel32_InitializeSListHead::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return true;
}



void Kernel32_SetUnhandledExceptionFilter::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	//We don't need this...
	//uint64_t virtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
	//mTopLevelExceptionFilter = GetAddressOfVirtualAddress (cpu, virtualAddress);
}

void Kernel32_SetUnhandledExceptionFilter::Call () {
	//We don't need this...
	//mLastTopLevelExceptionFilter = (uint64_t) SetUnhandledExceptionFilter ((LPTOP_LEVEL_EXCEPTION_FILTER) mTopLevelExceptionFilter);
}

bool Kernel32_SetUnhandledExceptionFilter::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, mLastTopLevelExceptionFilter, BX_64BIT_REG_RAX);
}
