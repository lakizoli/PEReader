// dllmain.cpp : Defines the entry point for the DLL application.
//#include "stdafx.h"

#include <Windows.h>
#include <cstdint>
#include <iostream>

HMODULE g_moduleHandle = nullptr;

int32_t (*g_StringFromGUID2) (const GUID* id, wchar_t* str, int32_t cmax) = nullptr;
long (*g_CoCreateGuid) (GUID* pguid) = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
	case DLL_PROCESS_ATTACH:
		std::cout << "StubOle32 DLL_PROCESS_ATTACH called" << std::endl;

		g_moduleHandle = LoadLibraryA ("C:\\WINDOWS\\System32\\ole32.dll");
		if (g_moduleHandle) {
			g_StringFromGUID2 = (int32_t (*) (const GUID* id, wchar_t* str, int32_t cmax)) GetProcAddress (g_moduleHandle, "StringFromGUID2");
			if (!g_StringFromGUID2) {
				return FALSE;
			}

			g_CoCreateGuid = (long (*) (GUID* pguid)) GetProcAddress (g_moduleHandle, "CoCreateGuid");
			if (!g_CoCreateGuid) {
				return FALSE;
			}
		}
		break;
    case DLL_THREAD_ATTACH:
		std::cout << "StubOle32 DLL_THREAD_ATTACH called" << std::endl;
		break;
    case DLL_THREAD_DETACH:
		std::cout << "StubOle32 DLL_THREAD_DETACH called" << std::endl;
		break;
    case DLL_PROCESS_DETACH:
		std::cout << "StubOle32 DLL_PROCESS_DETACH called" << std::endl;

		if (g_moduleHandle) {
			FreeLibrary (g_moduleHandle);
			g_moduleHandle = nullptr;
		}
        break;
    }
    return TRUE;
}

