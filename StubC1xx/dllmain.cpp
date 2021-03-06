// dllmain.cpp : Defines the entry point for the DLL application.

#include <windows.h>
#include <cstdint>
#include <iostream>

HMODULE g_moduleHandle = nullptr;

BOOL (*g_InvokeCompilerPassW) (int argc, wchar_t** argv, DWORD flags, HMODULE* phCLUIMod) = nullptr;
void (*g_AbortCompilerPass) (int how) = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
	case DLL_PROCESS_ATTACH:
		std::cout << "StubC1xx DLL_PROCESS_ATTACH called" << std::endl;

		g_moduleHandle = LoadLibraryA ("c1xx.dll.bin");
		if (g_moduleHandle) {
			g_InvokeCompilerPassW = (BOOL (*) (int argc, wchar_t** argv, DWORD flags, HMODULE* phCLUIMod)) GetProcAddress (g_moduleHandle, "InvokeCompilerPassW");
			if (!g_InvokeCompilerPassW) {
				return FALSE;
			}

			g_AbortCompilerPass = (void (*) (int how)) GetProcAddress (g_moduleHandle, "AbortCompilerPass");
			if (!g_AbortCompilerPass) {
				return FALSE;
			}
		}
		break;
	case DLL_THREAD_ATTACH:
		std::cout << "StubC1xx DLL_THREAD_ATTACH called" << std::endl;
		break;
	case DLL_THREAD_DETACH:
		std::cout << "StubC1xx DLL_THREAD_DETACH called" << std::endl;
		break;
	case DLL_PROCESS_DETACH:
		std::cout << "StubC1xx DLL_PROCESS_DETACH called" << std::endl;

		if (g_moduleHandle) {
			FreeLibrary (g_moduleHandle);
			g_moduleHandle = nullptr;
		}
		break;
	}
    return TRUE;
}

