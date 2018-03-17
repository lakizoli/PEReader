#include "stdafx.h"
#include "apicrtimports.hpp"

IMPLEMENT_IMPORT_HANDLER (ApiCrt_InitTerm, "[api-ms-win-crt-runtime-l1-1-0.dll]_initterm");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_InitTermE, "[api-ms-win-crt-runtime-l1-1-0.dll]_initterm_e");



extern "C" void _initterm (void*, void*);

void ApiCrt_InitTerm::ReadParameters (BX_CPU_C& cpu) {
	uint64_t virtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
	mFirstParam = GetAddressOfVirtualAddress (cpu, virtualAddress);

	virtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 1);
	mSecondParam = GetAddressOfVirtualAddress (cpu, virtualAddress);
}

void ApiCrt_InitTerm::Call () {
	_initterm (mFirstParam, mSecondParam);
}

bool ApiCrt_InitTerm::WriteResults (BX_CPU_C& cpu) {
	//... Nothing to do ...
	return true;
}



extern "C" int _initterm_e (void*, void*);
extern "C" void asm_jump_to_initterm_e (void*, void*, void*);

void ApiCrt_InitTermE::ReadParameters (BX_CPU_C& cpu) {
	uint64_t vaStart = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
	//mFirstParam = GetAddressOfVirtualAddress (cpu, virtualAddress);

	uint64_t vaEnd = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 1);
	//mSecondParam = GetAddressOfVirtualAddress (cpu, virtualAddress);

	uint64_t vaItem = vaStart;
	while (vaItem < vaEnd) {
		uint64_t vaCall = 0;
		if (!CopyFromVirtualMemory (cpu, vaItem, sizeof (uint64_t), vaCall)) {
			break;
		}

		vaItem += sizeof (uint64_t);
	}
}

void ApiCrt_InitTermE::Call () {
	asm_jump_to_initterm_e (&_initterm_e, (void*) mFirstParam, (void*) mSecondParam);
	mResultCode = _initterm_e ((void*) mFirstParam, (void*) mSecondParam);
}

bool ApiCrt_InitTermE::WriteResults (BX_CPU_C& cpu) {
	return WriteValueToGPRegister (cpu, mResultCode, BX_64BIT_REG_RAX);
}
