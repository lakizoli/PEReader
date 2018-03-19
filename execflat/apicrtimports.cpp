#include "stdafx.h"
#include "apicrtimports.hpp"

IMPLEMENT_IMPORT_HANDLER (ApiCrt_InitTerm, "[api-ms-win-crt-runtime-l1-1-0.dll]_initterm");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_InitTermE, "[api-ms-win-crt-runtime-l1-1-0.dll]_initterm_e");



extern "C" void _initterm (void*, void*);

void ApiCrt_InitTerm::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase) {
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



void ApiCrt_InitTermE::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase) {
	std::vector<uint8_t> InitTerm64Bit {
		0x48, 0x89, 0x5c, 0x24, 0x08,				//mov qword ptr ss : [rsp + 8], rbx
		0x48, 0x89, 0x74, 0x24, 0x10,				//mov qword ptr ss : [rsp + 16], rsi
		0x57,										//push rdi
		0x48, 0x83, 0xec, 0x20,						//sub rsp, 0x0000000000000020
		0x48, 0x8b, 0xf2,							//mov rsi, rdx
		0x48, 0x8b, 0xf9,							//mov rdi, rcx
		0x48, 0x3b, 0xca,							//cmp rcx, rdx
		0x74, 0x44,									//jz . + 68 (0x0000000000002046); local target : 0x000000000000d11e
		0x48, 0x8b, 0x1f,							//mov rbx, qword ptr ds : [rdi]
		0x48, 0x85, 0xdb,							//test rbx, rbx
		0x74, 0x33,									//jz . + 51 (0x0000000000002035); local target : 0x000000000000d115
		/*0x48, 0x8d, 0x05, 0x77, 0x03, 0x00, 0x00,*/	//replaced with nop -> lea rax, qword ptr ds : [rip + 887]
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x48, 0x3b, 0xd8,*/							//replaced with nop -> cmp rbx, rax
		0x90, 0x90, 0x90,
		/*0x74, 0x42,*/									//replaced with nop -> jz . + 66 (0x0000000000002044); local target : 0x000000000000d130
		0x90, 0x90,
		/*0x48, 0x8d, 0x05, 0x0b, 0x03, 0x00, 0x00,*/	//replaced with nop -> lea rax, qword ptr ds : [rip + 779]
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x48, 0x3b, 0xd8,*/							//replaced with nop -> cmp rbx, rax
		0x90, 0x90, 0x90,
		/*0x74, 0x3d,*/									//replaced with nop -> jz . + 61 (0x000000000000203f); local target : 0x000000000000d137
		0x90, 0x90,
		/*0x48, 0x8d, 0x05, 0x4f, 0x00, 0x00, 0x00,*/	//replaced with nop -> lea rax, qword ptr ds : [rip + 79]
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x48, 0x3b, 0xd8,*/							//replaced with nop -> cmp rbx, rax
		0x90, 0x90, 0x90,
		/*0x74, 0x38,*/									//replaced with nop -> jz . + 56 (0x000000000000203a); local target : 0x000000000000d13e
		0x90, 0x90,
		/*0x48, 0x8b, 0xcb,*/							//replaced with nop -> mov rcx, rbx
		0x90, 0x90, 0x90,
		/*0xff, 0x15, 0x39, 0xd4, 0x09, 0x00,*/			//replaced with nop -> call qword ptr ds : [rip + 644153]; data segment address at : 0x00000000000aa548
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0xff, 0xd3,									//call rbx; dynamic
		0x85, 0xc0,									//test eax, eax
		0x75, 0x0b,									//jnz . + 11 (0x000000000000200d); local target : 0x000000000000d120
		0x48, 0x83, 0xc7, 0x08,						//add rdi, 0x0000000000000008
		0x48, 0x3b, 0xfe,							//cmp rdi, rsi
		0x75, 0xbc,									//jnz . - 68 (0x0000000000001fbe); local target : 0x000000000000d0da
		0x33, 0xc0,									//xor eax, eax
		0x48, 0x8b, 0x5c, 0x24, 0x30,				//mov rbx, qword ptr ss : [rsp + 48]
		0x48, 0x8b, 0x74, 0x24, 0x38,				//mov rsi, qword ptr ss : [rsp + 56]
		0x48, 0x83, 0xc4, 0x20,						//add rsp, 0x0000000000000020
		0x5f,										//pop rdi
		0xc3,										//ret
	};

	cpu.cpu_inject_code (injectBase, &InitTerm64Bit[0], (Bit32u) InitTerm64Bit.size ());
	mImportFunctionAddress = injectBase;
}

void ApiCrt_InitTermE::Call () {
	//... Nothing to do ...
}

bool ApiCrt_InitTermE::WriteResults (BX_CPU_C& cpu) {
	return WriteValueToGPRegister (cpu, mImportFunctionAddress, BX_64BIT_REG_RIP);
}
