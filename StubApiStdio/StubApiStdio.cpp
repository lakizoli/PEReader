// StubApiStdio.cpp : Defines the exported functions for the DLL application.
//

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "defs.hpp"

extern unsigned int* (*g___p__commode) (void);
extern int (*g__set_fmode) (int mode);
extern FILE*  (*g_fopen) (const char* path, const char* mode);
extern long (*g__filelength) (int fd);
extern int (*g__write) (int fd, const void* buf, unsigned int count);
extern int (*g__isatty) (int fd);
extern int (*g_fputws) (const wchar_t* s, FILE* file);
extern intptr_t (*g__get_osfhandle) (int fd);
extern unsigned short (*g_getwchar) (void);
extern int (*g__dup2) (int od, int nd);
extern int (*g_fflush) (FILE* file);
extern int (*g__flushall) (void);
extern int (*g__wfopen_s) (FILE** pFile, const wchar_t* filename, const wchar_t* mode);
extern int (*g_fclose) (FILE* file);
extern int (*g___stdio_common_vswscanf) (unsigned __int64 options, const wchar_t* input, size_t length, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist);
extern int (*g___stdio_common_vswprintf_s) ( unsigned __int64 options, wchar_t* str, size_t count, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist);
extern size_t (*g_fread) (void* ptr, size_t size, size_t nmemb, FILE* file);
extern int (*g_feof) (FILE* file);
extern wchar_t* (*g_fgetws) (wchar_t* s, int size, FILE* file);
extern long (*g_ftell) (FILE* file);
extern size_t (*g_fwrite) (const void* ptr, size_t size, size_t nmemb, FILE* file);
extern int (*g___stdio_common_vswprintf) ( unsigned __int64 options, wchar_t* str, size_t len, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist);
extern int (*g__fileno) (FILE* file);
extern int (*g___stdio_common_vfwprintf) (unsigned __int64 options, FILE* file, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist);
extern int (*g___stdio_common_vsnwprintf_s) ( unsigned __int64 options, wchar_t* str, size_t sizeOfBuffer, size_t count, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist);
extern size_t (*g_fread_s) (void* buf, size_t buf_size, size_t elem_size, size_t count, FILE* stream);
extern int (*g_fseek) (FILE* file, int offset, int whence);
extern FILE* (*g__wfsopen) (const wchar_t* path, const wchar_t* mode, int share);

int g___p__commodeCounter = 0;
int g__set_fmodeCounter = 0;
int g_fopenCounter = 0;
int g__filelengthCounter = 0;
int g__writeCounter = 0;
int g__isattyCounter = 0;
int g_fputwsCounter = 0;
int g__get_osfhandleCounter = 0;
int g_getwcharCounter = 0;
int g__dup2Counter = 0;
int g_fflushCounter = 0;
int g__flushallCounter = 0;
int g__wfopen_sCounter = 0;
int g_fcloseCounter = 0;
int g___stdio_common_vswscanfCounter = 0;
int g___stdio_common_vswprintf_sCounter = 0;
int g_freadCounter = 0;
int g_feofCounter = 0;
int g_fgetwsCounter = 0;
int g_ftellCounter = 0;
int g_fwriteCounter = 0;
int g___stdio_common_vswprintfCounter = 0;
int g__filenoCounter = 0;
int g___stdio_common_vfwprintfCounter = 0;
int g___stdio_common_vsnwprintf_sCounter = 0;
int g_fread_sCounter = 0;
int g_fseekCounter = 0;
int g__wfsopenCounter = 0;

extern "C" void LogCall (int& counter, const char* log);
extern "C" void ConvertToMBCS (char* buffer, const wchar_t* wcs, size_t maxlen);

extern "C" __declspec(dllexport) unsigned int* __p__commode (void) {
	LogCall (g___p__commodeCounter, "__p__commode");

	if (!g___p__commode) {
		return 0;
	}
	return g___p__commode ();
}

extern "C" __declspec(dllexport) int _set_fmode (int mode) {
	LogCall (g__set_fmodeCounter, "_set_fmode");

	if (!g__set_fmode) {
		return 0;
	}
	return g__set_fmode (mode);
}

extern "C" __declspec(dllexport) FILE* fopen (const char* path, const char* mode) {
	char buffer[16 * 1024];
	memset (buffer, 0, sizeof (buffer));
	strcat (buffer, "fopen -> ");
	strcat (buffer, path);
	LogCall (g_fopenCounter, buffer);

	if (!g_fopen) {
		return 0;
	}
	return g_fopen (path, mode);
}

extern "C" __declspec(dllexport) long _filelength (int fd) {
	LogCall (g__filelengthCounter, "_filelength");

	if (!g__filelength) {
		return 0;
	}
	return g__filelength (fd);
}

extern "C" __declspec(dllexport) int _write (int fd, const void* buf, unsigned int count) {
	LogCall (g__writeCounter, "_write");

	if (!g__write) {
		return 0;
	}
	return g__write (fd, buf, count);
}

extern "C" __declspec(dllexport) int _isatty (int fd) {
	LogCall (g__isattyCounter, "_isatty");

	if (!g__isatty) {
		return 0;
	}
	return g__isatty (fd);
}

extern "C" __declspec(dllexport) int fputws (const wchar_t* s, FILE* file) {
	LogCall (g_fputwsCounter, "fputws");

	if (!g_fputws) {
		return 0;
	}
	return g_fputws (s, file);
}

extern "C" __declspec(dllexport) intptr_t _get_osfhandle (int fd) {
	LogCall (g__get_osfhandleCounter, "_get_osfhandle");

	if (!g__get_osfhandle) {
		return 0;
	}
	return g__get_osfhandle (fd);
}

extern "C" __declspec(dllexport) unsigned short getwchar (void) {
	LogCall (g_getwcharCounter, "getwchar");

	if (!g_getwchar) {
		return 0;
	}
	return g_getwchar ();
}

extern "C" __declspec(dllexport) int _dup2 (int od, int nd) {
	LogCall (g__dup2Counter, "_dup2");

	if (!g__dup2) {
		return 0;
	}
	return g__dup2 (od, nd);
}

extern "C" __declspec(dllexport) int fflush (FILE* file) {
	LogCall (g_fflushCounter, "fflush");

	if (!g_fflush) {
		return 0;
	}
	return g_fflush (file);
}

extern "C" __declspec(dllexport) int _flushall (void) {
	LogCall (g__flushallCounter, "_flushall");

	if (!g__flushall) {
		return 0;
	}
	return g__flushall ();
}

extern "C" __declspec(dllexport) int _wfopen_s (FILE** pFile, const wchar_t* filename, const wchar_t* mode) {
	char pathBuffer[16 * 1024];
	memset (pathBuffer, 0, sizeof (pathBuffer));
	ConvertToMBCS (pathBuffer, filename, 8 * 1024);

	char buffer[16 * 1024];
	memset (buffer, 0, sizeof (buffer));
	strcat (buffer, "_wfopen_s -> ");
	strcat (buffer, pathBuffer);
	LogCall (g__wfopen_sCounter, buffer);

	if (!g__wfopen_s) {
		return 0;
	}
	return g__wfopen_s (pFile, filename, mode);
}

extern "C" __declspec(dllexport) int fclose (FILE* file) {
	LogCall (g_fcloseCounter, "fclose");

	if (!g_fclose) {
		return 0;
	}
	return g_fclose (file);
}

extern "C" __declspec(dllexport) int __stdio_common_vswscanf (unsigned __int64 options, const wchar_t* input, size_t length, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) {
	LogCall (g___stdio_common_vswscanfCounter, "__stdio_common_vswscanf");

	if (!g___stdio_common_vswscanf) {
		return 0;
	}
	return g___stdio_common_vswscanf (options, input, length, format, locale, valist);
}

extern "C" __declspec(dllexport) int __stdio_common_vswprintf_s ( unsigned __int64 options, wchar_t* str, size_t count, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) {
	LogCall (g___stdio_common_vswprintf_sCounter, "__stdio_common_vswprintf_s");

	if (!g___stdio_common_vswprintf_s) {
		return 0;
	}
	return g___stdio_common_vswprintf_s (options, str, count, format, locale, valist);
}

extern "C" __declspec(dllexport) size_t fread (void* ptr, size_t size, size_t nmemb, FILE* file) {
	LogCall (g_freadCounter, "fread");

	if (!g_fread) {
		return 0;
	}
	return g_fread (ptr, size, nmemb, file);
}

extern "C" __declspec(dllexport) int feof (FILE* file) {
	LogCall (g_feofCounter, "feof");

	if (!g_feof) {
		return 0;
	}
	return g_feof (file);
}

extern "C" __declspec(dllexport) wchar_t* fgetws (wchar_t* s, int size, FILE* file) {
	LogCall (g_fgetwsCounter, "fgetws");

	if (!g_fgetws) {
		return 0;
	}
	return g_fgetws (s, size, file);
}

extern "C" __declspec(dllexport) long ftell (FILE* file) {
	LogCall (g_ftellCounter, "ftell");

	if (!g_ftell) {
		return 0;
	}
	return g_ftell (file);
}

extern "C" __declspec(dllexport) size_t fwrite (const void* ptr, size_t size, size_t nmemb, FILE* file) {
	LogCall (g_fwriteCounter, "fwrite");

	if (!g_fwrite) {
		return 0;
	}
	return g_fwrite (ptr, size, nmemb, file);
}

extern "C" __declspec(dllexport) int __stdio_common_vswprintf ( unsigned __int64 options, wchar_t* str, size_t len, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) {
	LogCall (g___stdio_common_vswprintfCounter, "__stdio_common_vswprintf");

	if (!g___stdio_common_vswprintf) {
		return 0;
	}
	return g___stdio_common_vswprintf (options, str, len, format, locale, valist);
}

extern "C" __declspec(dllexport) int _fileno (FILE* file) {
	LogCall (g__filenoCounter, "_fileno");

	if (!g__fileno) {
		return 0;
	}
	return g__fileno (file);
}

extern "C" __declspec(dllexport) int __stdio_common_vfwprintf (unsigned __int64 options, FILE* file, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) {
	LogCall (g___stdio_common_vfwprintfCounter, "__stdio_common_vfwprintf");

	if (!g___stdio_common_vfwprintf) {
		return 0;
	}
	return g___stdio_common_vfwprintf (options, file, format, locale, valist);
}

extern "C" __declspec(dllexport) int __stdio_common_vsnwprintf_s ( unsigned __int64 options, wchar_t* str, size_t sizeOfBuffer, size_t count, const wchar_t* format, MSVCRT__locale_t locale, __ms_va_list valist) {
	LogCall (g___stdio_common_vsnwprintf_sCounter, "__stdio_common_vsnwprintf_s");

	if (!g___stdio_common_vsnwprintf_s) {
		return 0;
	}
	return g___stdio_common_vsnwprintf_s (options, str, sizeOfBuffer, count, format, locale, valist);
}

extern "C" __declspec(dllexport) size_t fread_s (void* buf, size_t buf_size, size_t elem_size, size_t count, FILE* stream) {
	LogCall (g_fread_sCounter, "fread_s");

	if (!g_fread_s) {
		return 0;
	}
	return g_fread_s (buf, buf_size, elem_size, count, stream);
}

extern "C" __declspec(dllexport) int fseek (FILE* file, int offset, int whence) {
	LogCall (g_fseekCounter, "fseek");

	if (!g_fseek) {
		return 0;
	}
	return g_fseek (file, offset, whence);
}

extern "C" __declspec(dllexport) FILE* _wfsopen (const wchar_t* path, const wchar_t* mode, int share) {
	char pathBuffer[16 * 1024];
	memset (pathBuffer, 0, sizeof (pathBuffer));
	ConvertToMBCS (pathBuffer, path, 8 * 1024);

	char buffer[16 * 1024];
	memset (buffer, 0, sizeof (buffer));
	strcat (buffer, "_wfsopen -> ");
	strcat (buffer, pathBuffer);
	LogCall (g__wfsopenCounter, buffer);

	if (!g__wfsopen) {
		return 0;
	}
	return g__wfsopen (path, mode, share);
}
