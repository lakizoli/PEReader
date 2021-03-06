// StubOle32.cpp : Defines the exported functions for the DLL application.
//

#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <iostream>
#include <ctime>

#define E_INVALIDARG  0x80070057L

typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;

extern int32_t (*g_StringFromGUID2) (const GUID* id, wchar_t* str, int32_t cmax);
extern long (*g_CoCreateGuid) (GUID* pguid);

int g_StringFromGUID2Counter = 0;
int g_CoCreateGuidCounter = 0;

static void LogCall (int& counter, const char* log) {
	++counter;

	FILE* flog = fopen ("d:\\work\\call_timing.log", "ab");
	if (flog) {
		clock_t clk = clock ();
		fprintf (flog, "[%.2f sec] StubApiEnv -> %s [%d]\n", (double) clk / (double) CLOCKS_PER_SEC, log, counter);
		fclose (flog);
	}
}

extern "C" __declspec(dllexport) int32_t StringFromGUID2 (const GUID* id, wchar_t* str, int32_t cmax) {
	LogCall (g_StringFromGUID2Counter, "StringFromGUID2");

	if (!g_StringFromGUID2) {
		return E_INVALIDARG;
	}
	return g_StringFromGUID2 (id, str, cmax);
}

extern "C" __declspec(dllexport) long CoCreateGuid (GUID* pguid) {
	LogCall (g_CoCreateGuidCounter, "CoCreateGuid");

	if (!g_CoCreateGuid) {
		return E_INVALIDARG;
	}
	return g_CoCreateGuid (pguid);
}