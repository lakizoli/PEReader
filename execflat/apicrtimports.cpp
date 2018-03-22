#include "stdafx.h"
#include "apicrtimports.hpp"

IMPLEMENT_IMPORT_HANDLER (ApiCrt_InitTerm, "[api-ms-win-crt-runtime-l1-1-0.dll]_initterm");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_InitTermE, "[api-ms-win-crt-runtime-l1-1-0.dll]_initterm_e");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_SetAppType, "[api-ms-win-crt-runtime-l1-1-0.dll]_set_app_type");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_AtExit, "[api-ms-win-crt-runtime-l1-1-0.dll]_crt_atexit");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_ConfigureNarrowArgv, "[api-ms-win-crt-runtime-l1-1-0.dll]_configure_narrow_argv");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_InitializeNarrowEnvironment, "[api-ms-win-crt-runtime-l1-1-0.dll]_initialize_narrow_environment");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_GetArgv, "[api-ms-win-crt-runtime-l1-1-0.dll]__p___argv");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_GetArgc, "[api-ms-win-crt-runtime-l1-1-0.dll]__p___argc");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_GetInitialNarrowEnvironment, "[api-ms-win-crt-runtime-l1-1-0.dll]_get_initial_narrow_environment");

IMPLEMENT_IMPORT_HANDLER (ApiCrt_SetFMode, "[api-ms-win-crt-stdio-l1-1-0.dll]_set_fmode");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_GetComMode, "[api-ms-win-crt-stdio-l1-1-0.dll]__p__commode");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_IobFunc, "[api-ms-win-crt-stdio-l1-1-0.dll]__acrt_iob_func");
IMPLEMENT_IMPORT_HANDLER (ApiCrt_CommonVFPrintf, "[api-ms-win-crt-stdio-l1-1-0.dll]__stdio_common_vfprintf");

IMPLEMENT_IMPORT_HANDLER (ApiCrt_ConfigThreadLocale, "[api-ms-win-crt-locale-l1-1-0.dll]_configthreadlocale");

IMPLEMENT_IMPORT_HANDLER (ApiCrt_SetNewMode, "[api-ms-win-crt-heap-l1-1-0.dll]_set_new_mode");



void ApiCrt_InitTerm::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	std::vector<uint8_t> initTerm64Bit {
		0x40, 0x55,                  					//push rbp
		0x56,                        					//push rsi
		0x57,                        					//push rdi
		0x48, 0x83, 0xec, 0x30,							//sub rsp, 0x0000000000000030
		0x33, 0xf6,										//xor esi, esi
		0x48, 0x8b, 0xea,								//mov rbp, rdx
		0x48, 0x2b, 0xe9,								//sub rbp, rcx
		0x48, 0x8b, 0xf9,								//mov rdi, rcx
		0x48, 0x83, 0xc5, 0x07,							//add rbp, 0x0000000000000007
		0x48, 0xc1, 0xed, 0x03,							//shr rbp, 0x03
		0x48, 0x3b, 0xca,								//cmp rcx, rdx
		0x48, 0x0f, 0x47, 0xee,							//cmovnbe rbp, rsi
		0x48, 0x85, 0xed,								//test rbp, rbp
		0x0f, 0x84, 0x94, 0x00, 0x00, 0x00,				//jz . + 148 (0x000000000000209a); local target : 0x000000000000dadf
		0x48, 0x89, 0x5c, 0x24, 0x50,					//mov qword ptr ss : [rsp + 80], rbx
		/*0x48, 0x8d, 0x05, 0x39, 0x13, 0x00, 0x00,*/	//replaced with nop -> lea rax, qword ptr ds : [rip + 4921]
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x89, 0x64, 0x24, 0x58,*/				//replaced with nop -> mov qword ptr ss : [rsp + 88], r12
		0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x8d, 0x25, 0x4d, 0x13, 0x00, 0x00,*/	//replaced with nop -> lea r12, qword ptr ds : [rip + 4941]
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x89, 0x6c, 0x24, 0x60,*/				//replaced with nop -> mov qword ptr ss : [rsp + 96], r13
		0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x8d, 0x2d, 0x21, 0x0f, 0x00, 0x00,*/	//replaced with nop -> lea r13, qword ptr ds : [rip + 3873]
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x89, 0x74, 0x24, 0x28,*/				//replaced with nop -> mov qword ptr ss : [rsp + 40], r14
		0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x8d, 0x35, 0xb5, 0x12, 0x00, 0x00,*/	//replaced with nop -> lea r14, qword ptr ds : [rip + 4789]
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x89, 0x7c, 0x24, 0x20,*/				//replaced with nop -> mov qword ptr ss : [rsp + 32], r15
		0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x8d, 0x3d, 0xc9, 0x11, 0x00, 0x00,*/	//replaced with nop -> lea r15, qword ptr ds : [rip + 4553]
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x48, 0x8b, 0x1f,								//mov rbx, qword ptr ds : [rdi]
		0x48, 0x85, 0xdb,								//test rbx, rbx
		0x74, 0x2b,										//jz . + 43 (0x000000000000202d); local target : 0x000000000000daba
		/*0x48, 0x3b, 0xd8,*/							//replaced with nop -> cmp rbx, rax
		0x90, 0x90, 0x90,
		/*0x74, 0x53,*/									//replaced with nop -> jz . + 83 (0x0000000000002055); local target : 0x000000000000dae7
		0x90, 0x90,
		/*0x49, 0x3b, 0xde,*/							//replaced with nop -> cmp rbx, r14
		0x90, 0x90, 0x90,
		/*0x74, 0x55,*/									//replaced with nop -> jz . + 85 (0x0000000000002057); local target : 0x000000000000daee
		0x90, 0x90,
		/*0x49, 0x3b, 0xdf,*/							//replaced with nop -> cmp rbx, r15
		0x90, 0x90, 0x90,
		/*0x74, 0x57,*/									//replaced with nop -> jz . + 87 (0x0000000000002059); local target : 0x000000000000daf5
		0x90, 0x90,
		/*0x49, 0x3b, 0xdc,*/							//replaced with nop -> cmp rbx, r12
		0x90, 0x90, 0x90,
		/*0x74, 0x59,*/									//replaced with nop -> jz . + 89 (0x000000000000205b); local target : 0x000000000000dafc
		0x90, 0x90,
		/*0x49, 0x3b, 0xdd,*/							//replaced with nop -> cmp rbx, r13
		0x90, 0x90, 0x90,
		/*0x74, 0x5b,*/									//replaced with nop -> jz . + 91 (0x000000000000205d); local target : 0x000000000000db03
		0x90, 0x90,
		/*0x48, 0x8b, 0xcb,*/							//replaced with nop -> mov rcx, rbx
		0x90, 0x90, 0x90,
		/*0xff, 0x15, 0x97, 0xca, 0x09, 0x00,*/			//replaced with nop -> call qword ptr ds : [rip + 641687]; data segment address at : 0x00000000000aa548
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0xff, 0xd3,										//call rbx; dynamic
		0x48, 0x8d, 0x05, 0xd6, 0x12, 0x00, 0x00,		//replaced with nop -> lea rax, qword ptr ds : [rip + 4822]
		0x48, 0x83, 0xc7, 0x08,							//add rdi, 0x0000000000000008
		0x48, 0xff, 0xc6,								//inc rsi
		0x48, 0x3b, 0xf5,								//cmp rsi, rbp
		0x75, 0xc1,										//jnz . - 63 (0x0000000000001fc3); local target : 0x000000000000da87
		/*0x4c, 0x8b, 0x7c, 0x24, 0x20,*/				//replaced with nop -> mov r15, qword ptr ss : [rsp + 32]
		0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x8b, 0x74, 0x24, 0x28,*/				//replaced with nop -> mov r14, qword ptr ss : [rsp + 40]
		0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x8b, 0x6c, 0x24, 0x60,*/				//replaced with nop -> mov r13, qword ptr ss : [rsp + 96]
		0x90, 0x90, 0x90, 0x90, 0x90,
		/*0x4c, 0x8b, 0x64, 0x24, 0x58,*/				//replaced with nop -> mov r12, qword ptr ss : [rsp + 88]
		0x90, 0x90, 0x90, 0x90, 0x90,
		0x48, 0x8b, 0x5c, 0x24, 0x50,					//mov rbx, qword ptr ss : [rsp + 80]
		0x48, 0x83, 0xc4, 0x30,							//add rsp, 0x0000000000000030
		0x5f,											//pop rdi
		0x5e,											//pop rsi
		0x5d,											//pop rbp
		0xc3,											//ret
	};

	cpu.cpu_inject_code (injectBase, &initTerm64Bit[0], (Bit32u) initTerm64Bit.size ());
	mImportFunctionAddress = injectBase;
}

void ApiCrt_InitTerm::Call () {
	//... Nothing to do ...
}

bool ApiCrt_InitTerm::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	return WriteValueToGPRegister (cpu, mImportFunctionAddress, BX_64BIT_REG_RIP);
}


void ApiCrt_InitTermE::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	std::vector<uint8_t> initTerm64Bit {
		0x48, 0x89, 0x5c, 0x24, 0x08,					//mov qword ptr ss : [rsp + 8], rbx
		0x48, 0x89, 0x74, 0x24, 0x10,					//mov qword ptr ss : [rsp + 16], rsi
		0x57,											//push rdi
		0x48, 0x83, 0xec, 0x20,							//sub rsp, 0x0000000000000020
		0x48, 0x8b, 0xf2,								//mov rsi, rdx
		0x48, 0x8b, 0xf9,								//mov rdi, rcx
		0x48, 0x3b, 0xca,								//cmp rcx, rdx
		0x74, 0x44,										//jz . + 68 (0x0000000000002046); local target : 0x000000000000d11e
		0x48, 0x8b, 0x1f,								//mov rbx, qword ptr ds : [rdi]
		0x48, 0x85, 0xdb,								//test rbx, rbx
		0x74, 0x33,										//jz . + 51 (0x0000000000002035); local target : 0x000000000000d115
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
		0xff, 0xd3,										//call rbx; dynamic
		0x85, 0xc0,										//test eax, eax
		0x75, 0x0b,										//jnz . + 11 (0x000000000000200d); local target : 0x000000000000d120
		0x48, 0x83, 0xc7, 0x08,							//add rdi, 0x0000000000000008
		0x48, 0x3b, 0xfe,								//cmp rdi, rsi
		0x75, 0xbc,										//jnz . - 68 (0x0000000000001fbe); local target : 0x000000000000d0da
		0x33, 0xc0,										//xor eax, eax
		0x48, 0x8b, 0x5c, 0x24, 0x30,					//mov rbx, qword ptr ss : [rsp + 48]
		0x48, 0x8b, 0x74, 0x24, 0x38,					//mov rsi, qword ptr ss : [rsp + 56]
		0x48, 0x83, 0xc4, 0x20,							//add rsp, 0x0000000000000020
		0x5f,											//pop rdi
		0xc3,											//ret
	};

	cpu.cpu_inject_code (injectBase, &initTerm64Bit[0], (Bit32u) initTerm64Bit.size ());
	mImportFunctionAddress = injectBase;
}

void ApiCrt_InitTermE::Call () {
	//... Nothing to do ...
}

bool ApiCrt_InitTermE::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	return WriteValueToGPRegister (cpu, mImportFunctionAddress, BX_64BIT_REG_RIP);
}



void ApiCrt_SetAppType::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mAppType = ReadSimpleParameter_64BitCallingCV<uint32_t> (cpu, 0);
	ReadState (state);
}

void ApiCrt_SetAppType::Call () {
	if (mState) {
		mState->CRT_app_type = mAppType;
	}
}

bool ApiCrt_SetAppType::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	if (!WriteState (state)) {
		return false;
	}

	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return true;
}



void ApiCrt_AtExit::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mFuncAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
	ReadState (state);
}

void ApiCrt_AtExit::Call () {
	if (mState) {
		if (mState->CRT_atexit_registered >= mState->CRT_atexit_table_size) {
			mResult = -1;
		} else {
			mState->CRT_atexit_table[mState->CRT_atexit_registered++] = mFuncAddress;
			mResult = 0;
		}
	}
}

bool ApiCrt_AtExit::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	if (!WriteState (state)) {
		return false;
	}

	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, mResult, BX_64BIT_REG_RAX);
}



void ApiCrt_ConfigureNarrowArgv::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	//Nothing to do...
}

void ApiCrt_ConfigureNarrowArgv::Call () {
	//Nothing to do...
}

bool ApiCrt_ConfigureNarrowArgv::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, 0, BX_64BIT_REG_RAX);
}



void ApiCrt_InitializeNarrowEnvironment::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	//Nothing to do
}

void ApiCrt_InitializeNarrowEnvironment::Call () {
	//Nothing to do
}

bool ApiCrt_InitializeNarrowEnvironment::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, 0, BX_64BIT_REG_RAX);
}



void ApiCrt_GetArgv::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	ReadState (state);
}

void ApiCrt_GetArgv::Call () {
	if (mState) {
		mArgvVirtualAddress = GetVirtualAddressOfStateItem (offsetof (ApiCrtState, CRT___argv));
	}
}

bool ApiCrt_GetArgv::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//No writeback of state needed, because we read it only...

	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, mArgvVirtualAddress, BX_64BIT_REG_RAX);
}



void ApiCrt_GetArgc::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	ReadState (state);
}

void ApiCrt_GetArgc::Call () {
	if (mState) {
		mArgcVirtualAddress = GetVirtualAddressOfStateItem (offsetof (ApiCrtState, CRT___argc));
	}
}

bool ApiCrt_GetArgc::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//No writeback of state needed, because we read it only...

	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, mArgcVirtualAddress, BX_64BIT_REG_RAX);
}



void ApiCrt_GetInitialNarrowEnvironment::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	ReadState (state);
}

void ApiCrt_GetInitialNarrowEnvironment::Call () {
	if (mState) {
		mInitEnv = (uint64_t) mState->CRT___initenv;
	}
}

bool ApiCrt_GetInitialNarrowEnvironment::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//No writeback of state needed, because we read it only...

	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, mInitEnv, BX_64BIT_REG_RAX);
}



void ApiCrt_SetFMode::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mFileMode = ReadSimpleParameter_64BitCallingCV<uint32_t> (cpu, 0);
	ReadState (state);
}

void ApiCrt_SetFMode::Call () {
	if (mState) {
		mState->CRT__fmode = mFileMode;
	}
}

bool ApiCrt_SetFMode::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	if (!WriteState (state)) {
		return false;
	}

	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, 0, BX_64BIT_REG_RAX);
}



void ApiCrt_GetComMode::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	ReadState (state);
}

void ApiCrt_GetComMode::Call () {
	if (mState) {
		mComModeRes = GetVirtualAddressOfStateItem (offsetof (ApiCrtState, CRT__commode));
	}
}

bool ApiCrt_GetComMode::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//No writeback of state needed, because we read it only...

	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, mComModeRes, BX_64BIT_REG_RAX);
}



void ApiCrt_IobFunc::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mIobIndex = ReadSimpleParameter_64BitCallingCV<uint32_t> (cpu, 0);
}

void ApiCrt_IobFunc::Call () {
	mIobFunc = (uint64_t) __acrt_iob_func (mIobIndex);
}

bool ApiCrt_IobFunc::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	return WriteValueToGPRegister (cpu, mIobFunc, BX_64BIT_REG_RAX);
}



void ApiCrt_CommonVFPrintf::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mOptions = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 0);
	mFileParam = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 1);

	uint64_t virtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 2);
	mFormatParam = (const char*) GetAddressOfVirtualAddress (cpu, virtualAddress);

	virtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 3);
	mLocale = GetAddressOfVirtualAddress (cpu, virtualAddress);

	virtualAddress = ReadSimpleParameter_64BitCallingCV<uint64_t> (cpu, 4); //TODO: read stack...
	mFormatParamList = (va_list) GetAddressOfVirtualAddress (cpu, virtualAddress);
}

void ApiCrt_CommonVFPrintf::Call () {
	mPrintfResult = __stdio_common_vfprintf (mOptions, (FILE*) mFileParam, mFormatParam, (_locale_t) mLocale, mFormatParamList);
}

bool ApiCrt_CommonVFPrintf::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	return WriteValueToGPRegister (cpu, mPrintfResult, BX_64BIT_REG_RAX);
}



void ApiCrt_ConfigThreadLocale::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mType = ReadSimpleParameter_64BitCallingCV<int32_t> (cpu, 0);
}

void ApiCrt_ConfigThreadLocale::Call () {
	mResult = _configthreadlocale (mType);
}

bool ApiCrt_ConfigThreadLocale::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, mResult, BX_64BIT_REG_RAX);
}



void ApiCrt_SetNewMode::ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) {
	mMode = ReadSimpleParameter_64BitCallingCV<int32_t> (cpu, 0);
	ReadState (state);
}

void ApiCrt_SetNewMode::Call () {
	mState->CRT_new_mode = mMode;
}

bool ApiCrt_SetNewMode::WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) {
	if (!WriteState (state)) {
		return false;
	}

	//We have to call RET here to return to the caller after the jump call, because we ignored the call of the real function...
	cpu.RETnear64 (nullptr);

	return WriteValueToGPRegister (cpu, mOldMode, BX_64BIT_REG_RAX);
}
