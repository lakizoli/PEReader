// StubApiEnv.cpp : Defines the exported functions for the DLL application.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctime>

extern char* (*g_getenv) (const char *name);
extern int (*g__wgetenv_s) (size_t* pReturnValue, wchar_t* buffer, size_t numberOfElements, const wchar_t* varname);
extern int (*g__wputenv_s) (const wchar_t* name, const wchar_t* value);
extern wchar_t* (*g__wgetcwd) (wchar_t* buf, int size);
extern int (*g__wdupenv_s) (wchar_t** buffer, size_t* numberOfElements, const wchar_t* varname);

int g_getenvCounter = 0;
int g__wgetenv_sCounter = 0;
int g__wputenv_sCounter = 0;
int g__wgetcwdCounter = 0;
int g__wdupenv_sCounter = 0;

static void LogCall (int& counter, const char* log) {
	++counter;

	FILE* flog = fopen ("d:\\work\\call_timing.log", "ab");
	if (flog) {
		clock_t clk = clock ();
		fprintf (flog, "[%.2f sec] StubApiEnv -> %s [%d]\n", (double) clk / (double) CLOCKS_PER_SEC, log, counter);
		fclose (flog);
	}
}

extern "C" __declspec(dllexport) char* getenv (const char *name) {
	LogCall (g_getenvCounter, "getenv");

	if (!g_getenv) {
		return nullptr;
	}
	return g_getenv (name);
}

extern "C" __declspec(dllexport) int _wgetenv_s (size_t* pReturnValue, wchar_t* buffer, size_t numberOfElements, const wchar_t* varname) {
	LogCall (g__wgetenv_sCounter, "_wgetenv_s");

	if (!g__wgetenv_s) {
		return 0;
	}
	return g__wgetenv_s (pReturnValue, buffer, numberOfElements, varname);
}

extern "C" __declspec(dllexport) int _wputenv_s (const wchar_t* name, const wchar_t* value) {
	LogCall (g__wputenv_sCounter, "_wputenv_s");

	if (!g__wputenv_s) {
		return 0;
	}
	return g__wputenv_s (name, value);
}

extern "C" __declspec(dllexport) wchar_t* _wgetcwd (wchar_t* buf, int size) {
	LogCall (g__wgetcwdCounter, "_wgetcwd");


	if (!g__wgetcwd) {
		return 0;
	}
	return g__wgetcwd (buf, size);
}

extern "C" __declspec(dllexport) int _wdupenv_s (wchar_t** buffer, size_t* numberOfElements, const wchar_t* varname) {
	LogCall (g__wdupenv_sCounter, "_wdupenv_s");

	if (!g__wdupenv_s) {
		return 0;
	}
	return g__wdupenv_s (buffer, numberOfElements, varname);
}
