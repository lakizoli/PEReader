#include "stdafx.h"
#include "kernel32imports.hpp"

IMPLEMENT_IMPORT_HANDLER (Kernel32_GetSystemTimeAsFileTime, "[KERNEL32.dll]GetSystemTimeAsFileTime");
IMPLEMENT_IMPORT_HANDLER (Kernel32_GetCurrentThreadId, "[KERNEL32.dll]GetCurrentThreadId");
IMPLEMENT_IMPORT_HANDLER (Kernel32_GetCurrentProcessId, "[KERNEL32.dll]GetCurrentProcessId");
IMPLEMENT_IMPORT_HANDLER (Kernel32_QueryPerformanceCounter, "[KERNEL32.dll]QueryPerformanceCounter");



void Kernel32_GetSystemTimeAsFileTime::ReadParameters (BX_CPU_C& cpu) {
	mResultVirtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
}

void Kernel32_GetSystemTimeAsFileTime::Call () {
	GetSystemTimeAsFileTime (&mCallResult);
}

bool Kernel32_GetSystemTimeAsFileTime::WriteResults (BX_CPU_C& cpu) {
	return CopyToVirtualMemory (cpu, mCallResult, mResultVirtualAddress, sizeof (FILETIME));
}



void Kernel32_GetCurrentThreadId::ReadParameters (BX_CPU_C& cpu) {
	//No parameters needed
}

void Kernel32_GetCurrentThreadId::Call () {
	mThreadId = GetCurrentThreadId ();
}

bool Kernel32_GetCurrentThreadId::WriteResults (BX_CPU_C& cpu) {
	return WriteValueToGPRegister (cpu, (uint32_t) mThreadId, BX_64BIT_REG_RAX);
}



void Kernel32_GetCurrentProcessId::ReadParameters (BX_CPU_C& cpu) {
	//No parameters needed
}

void Kernel32_GetCurrentProcessId::Call () {
	mProcessId = GetCurrentProcessId ();
}

bool Kernel32_GetCurrentProcessId::WriteResults (BX_CPU_C& cpu) {
	return WriteValueToGPRegister (cpu, (uint32_t) mProcessId, BX_64BIT_REG_RAX);
}



void Kernel32_QueryPerformanceCounter::ReadParameters (BX_CPU_C& cpu) {
	mCounterVirtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
}

void Kernel32_QueryPerformanceCounter::Call () {
	mReturnValue = QueryPerformanceCounter (&mCounter);
}

bool Kernel32_QueryPerformanceCounter::WriteResults (BX_CPU_C& cpu) {
	if (!CopyToVirtualMemory (cpu, mCounter, mCounterVirtualAddress, sizeof (LARGE_INTEGER))) {
		return false;
	}

	return WriteValueToGPRegister (cpu, (uint32_t) mReturnValue, BX_64BIT_REG_RAX);
}

