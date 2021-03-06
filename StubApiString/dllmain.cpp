// dllmain.cpp : Defines the entry point for the DLL application.

#include <windows.h>
#include <iostream>

HMODULE g_moduleHandle = nullptr;

int (*g_iswdigit) (wchar_t wc) = nullptr;
int (*g_wcscat_s) (wchar_t* dst, size_t elem, const wchar_t* src) = nullptr;
wchar_t* (*g_wcspbrk) (const wchar_t* str, const wchar_t* accept) = nullptr;
int (*g_wcsspn) (const wchar_t* str, const wchar_t* accept) = nullptr;
int (*g_wcsncmp) (const wchar_t* str1, const wchar_t*  str2, int n) = nullptr;
int (*g_wcsncpy_s) (wchar_t* wcDest, size_t numElement, const wchar_t*wcSrc, size_t count) = nullptr;
int (*g_iswspace) (wchar_t wc) = nullptr;
int (*g__wcsupr_s) (wchar_t* str, size_t n) = nullptr;
int (*g__wcslwr_s) (wchar_t* str, size_t n) = nullptr;
int (*g__wcsicmp) (const wchar_t* str1, const wchar_t* str2) = nullptr;
wchar_t* (*g__wcsdup) (const wchar_t* str) = nullptr;
int (*g_wcsncat_s) (wchar_t* dst, size_t elem, const wchar_t* src, size_t count) = nullptr;
int (*g_towlower) (unsigned short c) = nullptr;
int (*g_wcscpy_s) (wchar_t* wcDest, size_t numElement, const wchar_t* wcSrc) = nullptr;
int (*g_wcscmp) (const wchar_t* str1, const wchar_t* str2) = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		std::cout << "StubApiString DLL_PROCESS_ATTACH called" << std::endl;

		g_moduleHandle = LoadLibraryA ("C:\\WINDOWS\\System32\\ucrtbase.dll");
		if (g_moduleHandle) {
			g_iswdigit = (int (*) (wchar_t wc)) GetProcAddress (g_moduleHandle, "iswdigit");
			if (!g_iswdigit) {
				return FALSE;
			}

			g_wcscat_s = (int (*) (wchar_t* dst, size_t elem, const wchar_t* src)) GetProcAddress (g_moduleHandle, "wcscat_s");
			if (!g_wcscat_s) {
				return FALSE;
			}
			
			g_wcspbrk = (wchar_t* (*) (const wchar_t* str, const wchar_t* accept)) GetProcAddress (g_moduleHandle, "wcspbrk");
			if (!g_wcspbrk) {
				return FALSE;
			}

			g_wcsspn = (int (*) (const wchar_t* str, const wchar_t* accept)) GetProcAddress (g_moduleHandle, "wcsspn");
			if (!g_wcsspn) {
				return FALSE;
			}

			g_wcsncmp = (int (*) (const wchar_t* str1, const wchar_t*  str2, int n)) GetProcAddress (g_moduleHandle, "wcsncmp");
			if (!g_wcsncmp) {
				return FALSE;
			}

			g_wcsncpy_s = (int (*) (wchar_t* wcDest, size_t numElement, const wchar_t*wcSrc, size_t count)) GetProcAddress (g_moduleHandle, "wcsncpy_s");
			if (!g_wcsncpy_s) {
				return FALSE;
			}

			g_iswspace = (int (*) (wchar_t wc)) GetProcAddress (g_moduleHandle, "iswspace");
			if (!g_iswspace) {
				return FALSE;
			}

			g__wcsupr_s = (int (*) (wchar_t* str, size_t n)) GetProcAddress (g_moduleHandle, "_wcsupr_s");
			if (!g__wcsupr_s) {
				return FALSE;
			}

			g__wcslwr_s = (int (*) (wchar_t* str, size_t n)) GetProcAddress (g_moduleHandle, "_wcslwr_s");
			if (!g__wcslwr_s) {
				return FALSE;
			}

			g__wcsicmp = (int (*) (const wchar_t* str1, const wchar_t* str2)) GetProcAddress (g_moduleHandle, "_wcsicmp");
			if (!g__wcsicmp) {
				return FALSE;
			}

			g__wcsdup = (wchar_t* (*) (const wchar_t* str)) GetProcAddress (g_moduleHandle, "_wcsdup");
			if (!g__wcsdup) {
				return FALSE;
			}

			g_wcsncat_s = (int (*) (wchar_t* dst, size_t elem, const wchar_t* src, size_t count)) GetProcAddress (g_moduleHandle, "wcsncat_s");
			if (!g_wcsncat_s) {
				return FALSE;
			}

			g_towlower = (int (*) (unsigned short c)) GetProcAddress (g_moduleHandle, "towlower");
			if (!g_towlower) {
				return FALSE;
			}

			g_wcscpy_s = (int (*) (wchar_t* wcDest, size_t numElement, const wchar_t* wcSrc)) GetProcAddress (g_moduleHandle, "wcscpy_s");
			if (!g_wcscpy_s) {
				return FALSE;
			}

			g_wcscmp = (int (*) (const wchar_t* str1, const wchar_t* str2)) GetProcAddress (g_moduleHandle, "wcscmp");
			if (!g_wcscmp) {
				return FALSE;
			}
		}
		break;
	case DLL_THREAD_ATTACH:
		std::cout << "StubApiString DLL_THREAD_ATTACH called" << std::endl;
		break;
	case DLL_THREAD_DETACH:
		std::cout << "StubApiString DLL_THREAD_DETACH called" << std::endl;
		break;
	case DLL_PROCESS_DETACH:
		std::cout << "StubApiString DLL_PROCESS_DETACH called" << std::endl;

		if (g_moduleHandle) {
			FreeLibrary (g_moduleHandle);
			g_moduleHandle = nullptr;
		}
		break;
    }
    return TRUE;
}

