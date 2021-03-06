// StubC2.cpp : Defines the exported functions for the DLL application.
//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string>
#include <vector>
#include <ctime>

extern BOOL (*g_InvokeCompilerPassW) (int argc, wchar_t** argv, DWORD flags, HMODULE* phCLUIMod);
extern void (*g_AbortCompilerPass) (int how);

int g_InvokeCompilerPassWCounter = 0;
int g_AbortCompilerPassCounter = 0;

static void LogCall (int& counter, const std::string& log) {
	++counter;

	FILE* flog = fopen ("d:\\work\\call_timing.log", "ab");
	if (flog) {
		clock_t clk = clock ();
		fprintf (flog, "[%.2f sec] StubC2 -> %s [%d]\n", (double) clk / (double) CLOCKS_PER_SEC, log.c_str (), counter);
		fclose (flog);
	}
}

static std::string ConvertWide (const std::wstring& log) {
	std::vector<char> buffer (log.size ());
	std::wcstombs (&buffer[0], &log[0], log.size ());
	return std::string (buffer.begin (), buffer.end ());
}

static std::string FileNameFromModuleHandle (HMODULE moduleHandle) {
	char buffer[MAX_PATH + 1];
	if (moduleHandle > 0) {
		DWORD len = GetModuleFileNameA (moduleHandle, buffer, MAX_PATH);
		if (len > 0) {
			return std::string (buffer, buffer + len);
		}
	}
	return std::string ("<empty>");
}

extern "C" __declspec(dllexport) BOOL InvokeCompilerPassW (int argc, wchar_t** argv, DWORD flags, HMODULE* phCLUIMod) {
	std::string log = "InvokeCompilerPassW -> argc: " + std::to_string (argc);
	for (int i = 0; i < argc && argv; ++i) {
		log += "\n, argv[" + std::to_string (i) + "]: " + ConvertWide (argv[i] == nullptr ? L"null" : argv[i]);
	}
	log += "\n, flags: " + std::to_string (flags);
	log += "\n, phCLUIMod: " + std::to_string ((unsigned long long) phCLUIMod);
	if (phCLUIMod) {
		log += "\n, cluimod filename: " + FileNameFromModuleHandle (*phCLUIMod);
	}

	LogCall (g_InvokeCompilerPassWCounter, log);

	if (!g_InvokeCompilerPassW) {
		return 0;
	}
	return g_InvokeCompilerPassW (argc, argv, flags, phCLUIMod);
}

extern "C" __declspec(dllexport) void AbortCompilerPass (int how) {
	LogCall (g_AbortCompilerPassCounter, "AbortCompilerPass");

	if (!g_AbortCompilerPass) {
		return;
	}
	g_AbortCompilerPass (how);
}
