// dllmain.cpp : Defines the entry point for the DLL application.

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <ctime>
#include "defs.hpp"

HMODULE g_moduleHandle = nullptr;

unsigned int* (*g___p__commode) (void) = nullptr;
int (*g__set_fmode) (int mode) = nullptr;
FILE*  (*g_fopen) (const char* path, const char* mode) = nullptr;
long (*g__filelength) (int fd) = nullptr;
int (*g__write) (int fd, const void* buf, unsigned int count) = nullptr;
int (*g__isatty) (int fd) = nullptr;
int (*g_fputws) (const wchar_t* s, FILE* file) = nullptr;
intptr_t (*g__get_osfhandle) (int fd) = nullptr;
unsigned short (*g_getwchar) (void) = nullptr;
int (*g__dup2) (int od, int nd) = nullptr;
int (*g_fflush) (FILE* file) = nullptr;
int (*g__flushall) (void) = nullptr;
int (*g__wfopen_s) (FILE** pFile, const wchar_t* filename, const wchar_t* mode) = nullptr;
int (*g_fclose) (FILE* file) = nullptr;
int (*g___stdio_common_vswscanf) (unsigned __int64 options, const wchar_t* input, size_t length, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) = nullptr;
int (*g___stdio_common_vswprintf_s) ( unsigned __int64 options, wchar_t* str, size_t count, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) = nullptr;
size_t (*g_fread) (void* ptr, size_t size, size_t nmemb, FILE* file) = nullptr;
int (*g_feof) (FILE* file) = nullptr;
wchar_t* (*g_fgetws) (wchar_t* s, int size, FILE* file) = nullptr;
long (*g_ftell) (FILE* file) = nullptr;
size_t (*g_fwrite) (const void* ptr, size_t size, size_t nmemb, FILE* file) = nullptr;
int (*g___stdio_common_vswprintf) ( unsigned __int64 options, wchar_t* str, size_t len, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) = nullptr;
int (*g__fileno) (FILE* file) = nullptr;
int (*g___stdio_common_vfwprintf) (unsigned __int64 options, FILE* file, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) = nullptr;
int (*g___stdio_common_vsnwprintf_s) ( unsigned __int64 options, wchar_t* str, size_t sizeOfBuffer, size_t count, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) = nullptr;
size_t (*g_fread_s) (void* buf, size_t buf_size, size_t elem_size, size_t count, FILE* stream) = nullptr;
int (*g_fseek) (FILE* file, int offset, int whence) = nullptr;
FILE* (*g__wfsopen) (const wchar_t* path, const wchar_t* mode, int share) = nullptr;

extern "C" void LogCall (int& counter, const char* log) {
	++counter;

	if (g_fopen && g_fwrite && g_fclose) {
		FILE* flog = g_fopen ("d:\\work\\call_timing.log", "ab");
		if (flog) {
			clock_t clk = clock ();

			char buffer[128*1024];
			unsigned int len = sprintf_s (buffer, 128*1024, "[%.2f sec] StubApiStdio2 -> %s [%d]\n", (double) clk / (double) CLOCKS_PER_SEC, log, counter);

			g_fwrite (buffer, sizeof (char), len, flog);
			g_fclose (flog);
		}
	}
}

extern "C" void ConvertToMBCS (char* buffer, const wchar_t* wcs, size_t maxlen) {
	wcstombs (buffer, wcs, maxlen);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "StubApiStdio DLL_PROCESS_ATTACH called" << std::endl;

        g_moduleHandle = LoadLibraryA ("C:\\WINDOWS\\System32\\ucrtbase.dll");
        if (g_moduleHandle) {
            g___p__commode = (unsigned int* (*) (void)) GetProcAddress (g_moduleHandle, "__p__commode");
            if (!g___p__commode) {
                return FALSE;
            }

            g__set_fmode = (int (*) (int mode)) GetProcAddress (g_moduleHandle, "_set_fmode");
            if (!g__set_fmode) {
                return FALSE;
            }

            g_fopen = (FILE*  (*) (const char* path, const char* mode)) GetProcAddress (g_moduleHandle, "fopen");
            if (!g_fopen) {
                return FALSE;
            }

            g__filelength = (long (*) (int fd)) GetProcAddress (g_moduleHandle, "_filelength");
            if (!g__filelength) {
                return FALSE;
            }

            g__write = (int (*) (int fd, const void* buf, unsigned int count)) GetProcAddress (g_moduleHandle, "_write");
            if (!g__write) {
                return FALSE;
            }

            g__isatty = (int (*) (int fd)) GetProcAddress (g_moduleHandle, "_isatty");
            if (!g__isatty) {
                return FALSE;
            }

            g_fputws = (int (*) (const wchar_t* s, FILE* file)) GetProcAddress (g_moduleHandle, "fputws");
            if (!g_fputws) {
                return FALSE;
            }

            g__get_osfhandle = (intptr_t (*) (int fd)) GetProcAddress (g_moduleHandle, "_get_osfhandle");
            if (!g__get_osfhandle) {
                return FALSE;
            }

			g_getwchar = (unsigned short (*) (void)) GetProcAddress (g_moduleHandle, "getwchar");
            if (!g_getwchar) {
                return FALSE;
            }

            g__dup2 = (int (*) (int od, int nd)) GetProcAddress (g_moduleHandle, "_dup2");
            if (!g__dup2) {
                return FALSE;
            }

            g_fflush = (int (*) (FILE* file)) GetProcAddress (g_moduleHandle, "fflush");
            if (!g_fflush) {
                return FALSE;
            }

            g__flushall = (int (*) (void)) GetProcAddress (g_moduleHandle, "_flushall");
            if (!g__flushall) {
                return FALSE;
            }

            g__wfopen_s = (int (*) (FILE** pFile, const wchar_t* filename, const wchar_t* mode)) GetProcAddress (g_moduleHandle, "_wfopen_s");
            if (!g__wfopen_s) {
                return FALSE;
            }

            g_fclose = (int (*) (FILE* file)) GetProcAddress (g_moduleHandle, "fclose");
            if (!g_fclose) {
                return FALSE;
            }

            g___stdio_common_vswscanf = (int (*) (unsigned __int64 options, const wchar_t* input, size_t length, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist)) GetProcAddress (g_moduleHandle, "__stdio_common_vswscanf");
            if (!g___stdio_common_vswscanf) {
                return FALSE;
            }

            g___stdio_common_vswprintf_s = (int (*) ( unsigned __int64 options, wchar_t* str, size_t count, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist)) GetProcAddress (g_moduleHandle, "__stdio_common_vswprintf_s");
            if (!g___stdio_common_vswprintf_s) {
                return FALSE;
            }

            g_fread = (size_t (*) (void* ptr, size_t size, size_t nmemb, FILE* file)) GetProcAddress (g_moduleHandle, "fread");
            if (!g_fread) {
                return FALSE;
            }

            g_feof = (int (*) (FILE* file)) GetProcAddress (g_moduleHandle, "feof");
            if (!g_feof) {
                return FALSE;
            }

            g_fgetws = (wchar_t* (*) (wchar_t* s, int size, FILE* file)) GetProcAddress (g_moduleHandle, "fgetws");
            if (!g_fgetws) {
                return FALSE;
            }

            g_ftell = (long (*) (FILE* file)) GetProcAddress (g_moduleHandle, "ftell");
            if (!g_ftell) {
                return FALSE;
            }

            g_fwrite = (size_t (*) (const void* ptr, size_t size, size_t nmemb, FILE* file)) GetProcAddress (g_moduleHandle, "fwrite");
            if (!g_fwrite) {
                return FALSE;
            }

            g___stdio_common_vswprintf = (int (*) ( unsigned __int64 options, wchar_t* str, size_t len, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist)) GetProcAddress (g_moduleHandle, "__stdio_common_vswprintf");
            if (!g___stdio_common_vswprintf) {
                return FALSE;
            }

            g__fileno = (int (*) (FILE* file)) GetProcAddress (g_moduleHandle, "_fileno");
            if (!g__fileno) {
                return FALSE;
            }

            g___stdio_common_vfwprintf = (int (*) (unsigned __int64 options, FILE* file, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist)) GetProcAddress (g_moduleHandle, "__stdio_common_vfwprintf");
            if (!g___stdio_common_vfwprintf) {
                return FALSE;
            }

            g___stdio_common_vsnwprintf_s = (int (*) ( unsigned __int64 options, wchar_t* str, size_t sizeOfBuffer, size_t count, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist)) GetProcAddress (g_moduleHandle, "__stdio_common_vsnwprintf_s");
            if (!g___stdio_common_vsnwprintf_s) {
                return FALSE;
            }

            g_fread_s = (size_t (*) (void* buf, size_t buf_size, size_t elem_size, size_t count, FILE* stream)) GetProcAddress (g_moduleHandle, "fread_s");
            if (!g_fread_s) {
                return FALSE;
            }

            g_fseek = (int (*) (FILE* file, int offset, int whence)) GetProcAddress (g_moduleHandle, "fseek");
            if (!g_fseek) {
                return FALSE;
            }

            g__wfsopen = (FILE* (*) (const wchar_t* path, const wchar_t* mode, int share)) GetProcAddress (g_moduleHandle, "_wfsopen");
            if (!g__wfsopen) {
                return FALSE;
            }
        }
        break;
    case DLL_THREAD_ATTACH:
        std::cout << "StubApiStdio DLL_THREAD_ATTACH called" << std::endl;
        break;
    case DLL_THREAD_DETACH:
        std::cout << "StubApiStdio DLL_THREAD_DETACH called" << std::endl;
        break;
    case DLL_PROCESS_DETACH:
        std::cout << "StubApiStdio DLL_PROCESS_DETACH called" << std::endl;

        if (g_moduleHandle) {
            FreeLibrary (g_moduleHandle);
            g_moduleHandle = nullptr;
        }
        break;
    }
    return TRUE;
}

