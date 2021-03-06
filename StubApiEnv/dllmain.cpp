// dllmain.cpp : Defines the entry point for the DLL application.

#include <Windows.h>
#include <iostream>

HMODULE g_moduleHandle = nullptr;

char* (*g_getenv) (const char *name) = nullptr;
int (*g__wgetenv_s) (size_t* pReturnValue, wchar_t* buffer, size_t numberOfElements, const wchar_t* varname) = nullptr;
int (*g__wputenv_s) (const wchar_t* name, const wchar_t* value) = nullptr;
wchar_t* (*g__wgetcwd) (wchar_t* buf, int size) = nullptr;
int (*g__wdupenv_s) (wchar_t** buffer, size_t* numberOfElements, const wchar_t* varname) = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		std::cout << "StubApiEnv DLL_PROCESS_ATTACH called" << std::endl;

		g_moduleHandle = LoadLibraryA ("C:\\WINDOWS\\System32\\ucrtbase.dll");
		if (g_moduleHandle) {
			g_getenv = (char* (*) (const char *name)) GetProcAddress (g_moduleHandle, "getenv");
			if (!g_getenv) {
				return FALSE;
			}

			g__wgetenv_s = (int (*) (size_t* pReturnValue, wchar_t* buffer, size_t numberOfElements, const wchar_t* varname)) GetProcAddress (g_moduleHandle, "_wgetenv_s");
			if (!g__wgetenv_s) {
				return FALSE;
			}

			g__wputenv_s = (int (*) (const wchar_t* name, const wchar_t* value)) GetProcAddress (g_moduleHandle, "_wputenv_s");
			if (!g__wputenv_s) {
				return FALSE;
			}

			g__wgetcwd = (wchar_t* (*) (wchar_t* buf, int size)) GetProcAddress (g_moduleHandle, "_wgetcwd");
			if (!g__wgetcwd) {
				return FALSE;
			}

			g__wdupenv_s = (int (*) (wchar_t** buffer, size_t* numberOfElements, const wchar_t* varname)) GetProcAddress (g_moduleHandle, "_wdupenv_s");
			if (!g__wdupenv_s) {
				return FALSE;
			}
		}
		break;
    case DLL_THREAD_ATTACH:
		std::cout << "StubApiEnv DLL_THREAD_ATTACH called" << std::endl;
		break;
    case DLL_THREAD_DETACH:
		std::cout << "StubApiEnv DLL_THREAD_DETACH called" << std::endl;
		break;
    case DLL_PROCESS_DETACH:
		std::cout << "StubApiEnv DLL_PROCESS_DETACH called" << std::endl;

		if (g_moduleHandle) {
			FreeLibrary (g_moduleHandle);
			g_moduleHandle = nullptr;
		}
		break;
    }
    return TRUE;
}

