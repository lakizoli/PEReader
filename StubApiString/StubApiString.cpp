// StubApiString.cpp : Defines the exported functions for the DLL application
//

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <ctime>

extern int (*g_iswdigit) (wchar_t wc);
extern int (*g_wcscat_s) (wchar_t* dst, size_t elem, const wchar_t* src);
extern wchar_t* (*g_wcspbrk) (const wchar_t* str, const wchar_t* accept);
extern int (*g_wcsspn) (const wchar_t* str, const wchar_t* accept);
extern int (*g_wcsncmp) (const wchar_t* str1, const wchar_t*  str2, int n);
extern int (*g_wcsncpy_s) (wchar_t* wcDest, size_t numElement, const wchar_t*wcSrc, size_t count);
extern int (*g_iswspace) (wchar_t wc);
extern int (*g__wcsupr_s) (wchar_t* str, size_t n);
extern int (*g__wcslwr_s) (wchar_t* str, size_t n);
extern int (*g__wcsicmp) (const wchar_t* str1, const wchar_t* str2);
extern wchar_t* (*g__wcsdup) (const wchar_t* str);
extern int (*g_wcsncat_s) (wchar_t* dst, size_t elem, const wchar_t* src, size_t count);
extern int (*g_towlower) (unsigned short c);
extern int (*g_wcscpy_s) (wchar_t* wcDest, size_t numElement, const wchar_t* wcSrc);
extern int (*g_wcscmp) (const wchar_t* str1, const wchar_t* str2);

int g_iswdigitCounter = 0;
int g_wcscat_sCounter = 0;
int g_wcspbrkCounter = 0;
int g_wcsspnCounter = 0;
int g_wcsncmpCounter = 0;
int g_wcsncpy_sCounter = 0;
int g_iswspaceCounter = 0;
int g__wcsupr_sCounter = 0;
int g__wcslwr_sCounter = 0;
int g__wcsicmpCounter = 0;
int g__wcsdupCounter = 0;
int g_wcsncat_sCounter = 0;
int g_towlowerCounter = 0;
int g_wcscpy_sCounter = 0;
int g_wcscmpCounter = 0;

static void LogCall (int& counter, const char* log) {
	++counter;

	//FILE* flog = fopen ("d:\\work\\call_timing.log", "ab");
	//if (flog) {
	//	clock_t clk = clock ();
	//	fprintf (flog, "[%.2f sec] StubApiEnv -> %s [%d]\n", (double) clk / (double) CLOCKS_PER_SEC, log, counter);
	//	fclose (flog);
	//}
}

extern "C" __declspec(dllexport) int iswdigit (wchar_t wc) {
	LogCall (g_iswdigitCounter, "iswdigit");

	if (!g_iswdigit) {
		return 0;
	}
	return g_iswdigit (wc);
}

extern "C" __declspec(dllexport) int wcscat_s (wchar_t* dst, size_t elem, const wchar_t* src) {
	LogCall (g_wcscat_sCounter, "wcscat_s");

	if (!g_wcscat_s) {
		return 0;
	}
	return g_wcscat_s (dst, elem, src);
}

extern "C" __declspec(dllexport) wchar_t* wcspbrk (const wchar_t* str, const wchar_t* accept) {
	LogCall (g_wcspbrkCounter, "wcspbrk");

	if (!g_wcspbrk) {
		return 0;
	}
	return g_wcspbrk (str, accept);
}

extern "C" __declspec(dllexport) int wcsspn (const wchar_t* str, const wchar_t* accept) {
	LogCall (g_wcsspnCounter, "wcsspn");

	if (!g_wcsspn) {
		return 0;
	}
	return g_wcsspn (str, accept);
}

extern "C" __declspec(dllexport) int wcsncmp (const wchar_t* str1, const wchar_t* str2, int n) {
	LogCall (g_wcsncmpCounter, "wcsncmp");

	if (!g_wcsncmp) {
		return 0;
	}
	return g_wcsncmp (str1, str2, n);
}

extern "C" __declspec(dllexport) int wcsncpy_s (wchar_t* wcDest, size_t numElement, const wchar_t* wcSrc, size_t count) {
	LogCall (g_wcsncmpCounter, "wcsncpy_s");

	if (!g_wcsncpy_s) {
		return 0;
	}
	return g_wcsncpy_s (wcDest, numElement, wcSrc, count);
}

extern "C" __declspec(dllexport) int iswspace (wchar_t wc) {
	LogCall (g_iswspaceCounter, "iswspace");

	if (!g_iswspace) {
		return 0;
	}
	return g_iswspace (wc);
}

extern "C" __declspec(dllexport) int _wcsupr_s (wchar_t* str, size_t n) {
	LogCall (g__wcsupr_sCounter, "_wcsupr_s");

	if (!g__wcsupr_s) {
		return 0;
	}
	return g__wcsupr_s (str, n);
}

extern "C" __declspec(dllexport) int _wcslwr_s (wchar_t* str, size_t n) {
	LogCall (g__wcslwr_sCounter, "_wcslwr_s");

	if (!g__wcslwr_s) {
		return 0;
	}
	return g__wcslwr_s (str, n);
}

extern "C" __declspec(dllexport) int _wcsicmp (const wchar_t* str1, const wchar_t* str2) {
	LogCall (g__wcsicmpCounter, "_wcsicmp");

	if (!g__wcsicmp) {
		return 0;
	}
	return g__wcsicmp (str1, str2);
}

extern "C" __declspec(dllexport) wchar_t* _wcsdup (const wchar_t* str) {
	LogCall (g__wcsdupCounter, "_wcsdup");

	if (!g__wcsdup) {
		return 0;
	}
	return g__wcsdup (str);
}

extern "C" __declspec(dllexport) int wcsncat_s (wchar_t* dst, size_t elem, const wchar_t* src, size_t count) {
	LogCall (g_wcsncat_sCounter, "wcsncat_s");

	if (!g_wcsncat_s) {
		return 0;
	}
	return g_wcsncat_s (dst, elem, src, count);
}

extern "C" __declspec(dllexport) int towlower (unsigned short c) {
	LogCall (g_towlowerCounter, "towlower");

	if (!g_towlower) {
		return 0;
	}
	return g_towlower (c);
}

extern "C" __declspec(dllexport) int wcscpy_s (wchar_t* wcDest, size_t numElement, const wchar_t* wcSrc) {
	LogCall (g_wcscpy_sCounter, "wcscpy_s");

	if (!g_wcscpy_s) {
		return 0;
	}
	return g_wcscpy_s (wcDest, numElement, wcSrc);
}

//extern "C" __declspec(dllexport) int wcscmp (const wchar_t* str1, const wchar_t* str2) {
//	LogCall (g_wcscmpCounter, "wcscmp");
//
//	if (!g_wcscmp) {
//		return 0;
//	}
//	return g_wcscmp (str1, str2);
//}
