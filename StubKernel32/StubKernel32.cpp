// StubKernel32.cpp : Defines the exported functions for the DLL application.
//

#define _CRT_SECURE_NO_WARNINGS
#include "defs.h"
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>

extern std::string g_logModule;

extern "C" BOOL (*g_CreateDirectoryW) (LPCWSTR path, void* sa);
extern "C" BOOL (*g_ReadFile) (HANDLE hFile, LPVOID buffer, DWORD bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped);
extern "C" LPWSTR (*g_GetCommandLineW) (void);
extern "C" HANDLE (*g_GetCurrentProcess) (void);
extern "C" DWORD (*g_GetModuleFileNameW) (HMODULE hModule, LPWSTR lpFileName, DWORD size);
extern "C" UINT (*g_SetErrorMode) (UINT mode);
extern "C" DWORD (*g_SetFilePointer) (HANDLE hFile, LONG distance, LONG *highword, DWORD method);
extern "C" DWORD (*g_GetEnvironmentVariableW) (LPCWSTR name, LPWSTR val, DWORD size);
extern "C" HANDLE (*g_CreateMutexW) (void* sa, BOOL owner, LPCWSTR name);
extern "C" BOOL (*g_InitializeCriticalSectionEx) (void* crit, DWORD spincount, DWORD flags);
extern "C" DWORD (*g_WaitForSingleObject) (HANDLE handle, DWORD timeout);
extern "C" HANDLE (*g_CreateFileW) (LPCWSTR filename, DWORD access, DWORD sharing, void* sa, DWORD creation, DWORD attributes, HANDLE templateParam);
extern "C" BOOL (*g_ReleaseMutex) (HANDLE handle);
extern "C" HANDLE (*g_CreateToolhelp32Snapshot) (DWORD flags, DWORD process);
extern "C" DWORD (*g_GetLastError) (void);
extern "C" BOOL (*g_Process32NextW) (HANDLE hSnapshot, void* lppe);
extern "C" BOOL (*g_Process32FirstW) (HANDLE hSnapshot, void* lppe);
extern "C" BOOL (*g_CloseHandle) (HANDLE handle);
extern "C" void (*g_RaiseException) (DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args);
extern "C" VOID (*g_GetSystemInfo) (LPSYSTEM_INFO si);
extern "C" HMODULE (*g_LoadLibraryW) (LPCWSTR libnameW);
extern "C" BOOL (*g_SwitchToThread) (void);
extern "C" void* (*g_DecodePointer) (void * ptr);
extern "C" FARPROC (*g_GetProcAddress) (HMODULE hModule, LPCSTR function);
extern "C" void (*g_DeleteCriticalSection) (void* crit);
extern "C" BOOL (*g_CreateProcessW) (LPCWSTR app_name, LPWSTR cmd_line, void* process_attr, void* thread_attr, BOOL inherit, DWORD flags, LPVOID env, LPCWSTR cur_dir, LPSTARTUPINFOW startup_info, LPPROCESS_INFORMATION info);
extern "C" HMODULE (*g_GetModuleHandleW) (LPCWSTR module);
extern "C" BOOL (*g_FreeLibrary) (HINSTANCE hLibModule);
extern "C" HANDLE (*g_OpenMutexW) (DWORD access, BOOL inherit, LPCWSTR name);
extern "C" SIZE_T (*g_VirtualQuery) (LPCVOID addr, PMEMORY_BASIC_INFORMATION info, SIZE_T len);
extern "C" HMODULE (*g_LoadLibraryExW) (LPCWSTR libnameW, HANDLE hfile, DWORD flags);
extern "C" DWORD (*g_GetTempPathW) (DWORD count, LPWSTR path);
extern "C" BOOL (*g_GetDiskFreeSpaceExW) (LPCWSTR root, PULARGE_INTEGER avail, PULARGE_INTEGER total, PULARGE_INTEGER totalfree);
extern "C" INT (*g_WideCharToMultiByte) (UINT page, DWORD flags, LPCWSTR src, INT srclen, LPSTR dst, INT dstlen, LPCSTR defchar, BOOL *used);
extern "C" BOOL (*g_SetConsoleCtrlHandler) (PHANDLER_ROUTINE func, BOOL add);
extern "C" BOOL (*g_GetConsoleScreenBufferInfo) (HANDLE hConsoleOutput, void* csbi);
extern "C" HANDLE (*g_OpenEventW) (DWORD access, BOOL inherit, LPCWSTR name);
extern "C" BOOL (*g_SetEvent) (HANDLE handle);
extern "C" UINT (*g_GetConsoleOutputCP) (VOID);
extern "C" BOOL (*g_WriteFile) (HANDLE hFile, LPCVOID buffer, DWORD bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped);
extern "C" UINT (*g_GetACP) (void);
extern "C" BOOL (*g_GetConsoleMode) (HANDLE hcon, LPDWORD mode);
extern "C" BOOL (*g_QueryPerformanceFrequency) (PLARGE_INTEGER frequency);
extern "C" HGLOBAL (*g_LoadResource) (HINSTANCE hModule, HRSRC hRsrc);
extern "C" HRSRC (*g_FindResourceW) (HINSTANCE hModule, LPCWSTR name, LPCWSTR type);
extern "C" DWORD (*g_GetFileType) (HANDLE hFile);
extern "C" BOOL (*g_QueryPerformanceCounter) (PLARGE_INTEGER counter);
extern "C" BOOL (*g_VirtualFree) (LPVOID addr, SIZE_T size, DWORD type);
extern "C" LPVOID (*g_VirtualAlloc) (void *addr, SIZE_T size, DWORD type, DWORD protect);
extern "C" BOOL (*g_UnmapViewOfFile) (LPCVOID addr);
extern "C" HANDLE (*g_CreateFileMappingW) (HANDLE hFile, void* sa, DWORD protect, DWORD size_high, DWORD size_low, LPCWSTR name);
extern "C" LPVOID (*g_MapViewOfFileEx) (HANDLE handle, DWORD access, DWORD offset_high, DWORD offset_low, SIZE_T count, LPVOID addr);
extern "C" VOID (*g_GetStartupInfoW) (LPSTARTUPINFOW info);
extern "C" NTSTATUS (*g_EnterCriticalSection) (void* crit);
extern "C" HANDLE (*g_GetStdHandle) (DWORD std_handle);
extern "C" DWORD (*g_WaitForMultipleObjects) (DWORD count, const HANDLE *handles, BOOL wait_all, DWORD timeout);
extern "C" BOOL (*g_SetEnvironmentVariableW) (LPCWSTR name, LPCWSTR value);
extern "C" BOOL (*g_SetThreadPriority) (HANDLE hthread, INT priority);
extern "C" NTSTATUS (*g_LeaveCriticalSection) (void* crit);
extern "C" BOOL (*g_CreatePipe) (PHANDLE hReadPipe, PHANDLE hWritePipe, void* sa, DWORD size);
extern "C" void (*g_InitializeCriticalSection) (void* crit);
extern "C" BOOL (*g_DuplicateHandle) (HANDLE source_process, HANDLE source, HANDLE dest_process, HANDLE *dest, DWORD access, BOOL inherit, DWORD options);
extern "C" VOID (*g_Sleep) (DWORD timeout);
extern "C" HANDLE (*g_CreateThread) (void* sa, SIZE_T stack, LPTHREAD_START_ROUTINE start, LPVOID param, DWORD flags, LPDWORD id);
extern "C" DWORD (*g_GetCurrentProcessId) (void);
extern "C" LPWSTR (*g_GetEnvironmentStringsW) (void);
extern "C" HANDLE (*g_FindFirstFileW) (LPCWSTR lpFileName, void* lpFindData);
extern "C" BOOLEAN (*g_HeapFree) (HANDLE heap, ULONG flags, PVOID ptr);
extern "C" BOOL (*g_FindNextFileW) (HANDLE handle, void* data);
extern "C" LANGID (*g_GetUserDefaultUILanguage) (void);
extern "C" PVOID (*g_EncodePointer) (PVOID ptr);
extern "C" BOOL (*g_FindClose) (HANDLE handle);
extern "C" PVOID (*g_HeapAlloc) (HANDLE heap, ULONG flags, SIZE_T size);
extern "C" HANDLE (*g_GetProcessHeap) (void);
extern "C" DWORD (*g_ExpandEnvironmentStringsW) (LPCWSTR src, LPWSTR dst, DWORD count);
extern "C" DWORD (*g_GetCurrentThreadId) (void);
extern "C" DWORD (*g_GetVersion) (void);
extern "C" HANDLE (*g_CreateEventW) (void* sa, BOOL manual_reset, BOOL initial_state, LPCWSTR name);
extern "C" BOOL (*g_IsDebuggerPresent) (void);
extern "C" VOID (*g_InitializeSListHead) (PSLIST_HEADER list);
extern "C" VOID (*g_GetSystemTimeAsFileTime) (LPFILETIME time);
extern "C" DWORD (*g_WaitForSingleObjectEx) (HANDLE handle, DWORD timeout, BOOL alertable);
extern "C" BOOL (*g_ResetEvent) (HANDLE handle);
extern "C" BOOL (*g_IsProcessorFeaturePresent) (DWORD feature);
extern "C" BOOL (*g_TerminateProcess) (HANDLE handle, DWORD exit_code);
extern "C" LPTOP_LEVEL_EXCEPTION_FILTER (*g_SetUnhandledExceptionFilter) (LPTOP_LEVEL_EXCEPTION_FILTER filter);
extern "C" LONG (*g_UnhandledExceptionFilter) (PEXCEPTION_POINTERS epointers);
extern "C" PVOID (*g_RtlVirtualUnwind) (ULONG type, ULONG64 base, ULONG64 pc, void* function, void* context, PVOID *data, ULONG64 *frame_ret, void* ctx_ptr);
extern "C" PRUNTIME_FUNCTION (*g_RtlLookupFunctionEntry) (ULONG64 pc, ULONG64 *base, void* table);
extern "C" VOID (*g_RtlCaptureContext) (VOID*);
extern "C" void (*g_OutputDebugStringW) (LPCWSTR str);
extern "C" BOOL (*g_VirtualProtect) (LPVOID addr, SIZE_T size, DWORD new_prot, LPDWORD old_prot);
extern "C" HMODULE (*g_LoadLibraryExA) (LPCSTR libname, HANDLE hfile, DWORD flags);

//extern "C" BOOL (*g_GetVolumeInformationW) (LPCWSTR root, LPWSTR label, DWORD label_len, DWORD *serial, DWORD *filename_len, DWORD *flags, LPWSTR fsname, DWORD fsname_len);
//extern "C" DWORD (*g_GetFinalPathNameByHandleW) (HANDLE file, LPWSTR path, DWORD charcount, DWORD flags);
//extern "C" UINT (*g_GetDriveTypeW) (LPCWSTR root);
//extern "C" DWORD (*g_FormatMessageW) (DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPWSTR lpBuffer, DWORD nSize, void* args);
//extern "C" int (*g_lstrcmpiW) (LPCWSTR str1, LPCWSTR str2);
//extern "C" INT (*g_MultiByteToWideChar) (UINT page, DWORD flags, LPCSTR src, INT srclen, LPWSTR dst, INT dstlen);
//extern "C" DWORD (*g_SizeofResource) (HINSTANCE hModule, HRSRC hRsrc);
//extern "C" BOOL (*g_GetCPInfo) (UINT codepage, LPCPINFO cpinfo);
//extern "C" BOOL (*g_IsValidCodePage) (UINT codepage);
//extern "C" BOOL (*g_GetVersionExW) (void* info);
//extern "C" HANDLE (*g_OpenFileMappingW) (DWORD access, BOOL inherit, LPCWSTR name);
//extern "C" LPVOID (*g_MapViewOfFile) (HANDLE mapping, DWORD access, DWORD offset_high, DWORD offset_low, SIZE_T count);
//extern "C" void (*g_RaiseFailFastException) (DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args);
//extern "C" void (*g_ExitProcess) (DWORD status);
//extern "C" BOOL (*g_ReleaseSemaphore) (HANDLE handle, LONG count, LONG *previous);
//extern "C" HANDLE (*g_CreateSemaphoreW) (void* sa, LONG initial, LONG max, LPCWSTR name);
//extern "C" DWORD (*g_GetFileSize) (HANDLE hFile, LPDWORD filesizehigh);
//extern "C" BOOL (*g_SetProcessWorkingSetSize) (HANDLE hProcess, SIZE_T minset, SIZE_T maxset);
//extern "C" INT (*g_LCMapStringW) (LCID lcid, DWORD flags, LPCWSTR src, INT srclen, LPWSTR dst, INT dstlen);
//extern "C" DWORD (*g_GetLongPathNameW) (LPCWSTR shortpath, LPWSTR longpath, DWORD longlen);
//extern "C" HANDLE (*g_OpenProcess) (DWORD access, BOOL inherit, DWORD id);
//extern "C" void (*g_SetLastError) (DWORD error);
//extern "C" HLOCAL (*g_LocalFree) (HLOCAL handle);
//extern "C" HLOCAL (*g_LocalAlloc) (UINT flags, SIZE_T size);
//extern "C" HMODULE (*g_LoadLibraryA) (LPCSTR libname);
//extern "C" BOOL (*g_LockFileEx) (HANDLE hFile, DWORD flags, DWORD reserved, DWORD count_low, DWORD count_high, LPOVERLAPPED overlapped);
//extern "C" BOOL (*g_UnlockFileEx) (HANDLE hFile, DWORD reserved, DWORD count_low, DWORD count_high, LPOVERLAPPED overlapped);
//extern "C" BOOL (*g_GetFileAttributesExW) (LPCWSTR name, GET_FILEEX_INFO_LEVELS level, LPVOID ptr);
//extern "C" BOOL (*g_GetFileTime) (HANDLE hFile, FILETIME *lpCreationTime, FILETIME *lpLastAccessTime, FILETIME *lpLastWriteTime);
//extern "C" BOOL (*g_SetFileTime) (HANDLE hFile, const FILETIME *ctime, const FILETIME *atime, const FILETIME *mtime);

int g_CreateDirectoryWCounter = 0;
int g_ReadFileCounter = 0;
int g_GetCommandLineWCounter = 0;
int g_GetCurrentProcessCounter = 0;
int g_GetModuleFileNameWCounter = 0;
int g_SetErrorModeCounter = 0;
int g_SetFilePointerCounter = 0;
int g_GetEnvironmentVariableWCounter = 0;
int g_CreateMutexWCounter = 0;
int g_InitializeCriticalSectionExCounter = 0;
int g_WaitForSingleObjectCounter = 0;
int g_CreateFileWCounter = 0;
int g_ReleaseMutexCounter = 0;
int g_CreateToolhelp32SnapshotCounter = 0;
int g_GetLastErrorCounter = 0;
int g_Process32NextWCounter = 0;
int g_Process32FirstWCounter = 0;
int g_CloseHandleCounter = 0;
int g_RaiseExceptionCounter = 0;
int g_GetSystemInfoCounter = 0;
int g_LoadLibraryWCounter = 0;
int g_SwitchToThreadCounter = 0;
int g_DecodePointerCounter = 0;
int g_GetProcAddressCounter = 0;
int g_DeleteCriticalSectionCounter = 0;
int g_CreateProcessWCounter = 0;
int g_GetModuleHandleWCounter = 0;
int g_FreeLibraryCounter = 0;
int g_OpenMutexWCounter = 0;
int g_VirtualQueryCounter = 0;
int g_LoadLibraryExWCounter = 0;
int g_GetTempPathWCounter = 0;
int g_GetDiskFreeSpaceExWCounter = 0;
int g_WideCharToMultiByteCounter = 0;
int g_SetConsoleCtrlHandlerCounter = 0;
int g_GetConsoleScreenBufferInfoCounter = 0;
int g_OpenEventWCounter = 0;
int g_SetEventCounter = 0;
int g_GetConsoleOutputCPCounter = 0;
int g_WriteFileCounter = 0;
int g_GetACPCounter = 0;
int g_GetConsoleModeCounter = 0;
int g_QueryPerformanceFrequencyCounter = 0;
int g_LoadResourceCounter = 0;
int g_FindResourceWCounter = 0;
int g_GetFileTypeCounter = 0;
int g_QueryPerformanceCounterCounter = 0;
int g_VirtualFreeCounter = 0;
int g_VirtualAllocCounter = 0;
int g_UnmapViewOfFileCounter = 0;
int g_CreateFileMappingWCounter = 0;
int g_MapViewOfFileExCounter = 0;
int g_GetStartupInfoWCounter = 0;
int g_EnterCriticalSectionCounter = 0;
int g_GetStdHandleCounter = 0;
int g_WaitForMultipleObjectsCounter = 0;
int g_SetEnvironmentVariableWCounter = 0;
int g_SetThreadPriorityCounter = 0;
int g_LeaveCriticalSectionCounter = 0;
int g_CreatePipeCounter = 0;
int g_InitializeCriticalSectionCounter = 0;
int g_DuplicateHandleCounter = 0;
int g_SleepCounter = 0;
int g_CreateThreadCounter = 0;
int g_GetCurrentProcessIdCounter = 0;
int g_GetEnvironmentStringsWCounter = 0;
int g_FindFirstFileWCounter = 0;
int g_HeapFreeCounter = 0;
int g_FindNextFileWCounter = 0;
int g_GetUserDefaultUILanguageCounter = 0;
int g_EncodePointerCounter = 0;
int g_FindCloseCounter = 0;
int g_HeapAllocCounter = 0;
int g_GetProcessHeapCounter = 0;
int g_ExpandEnvironmentStringsWCounter = 0;
int g_GetCurrentThreadIdCounter = 0;
int g_GetVersionCounter = 0;
int g_CreateEventWCounter = 0;
int g_IsDebuggerPresentCounter = 0;
int g_InitializeSListHeadCounter = 0;
int g_GetSystemTimeAsFileTimeCounter = 0;
int g_WaitForSingleObjectExCounter = 0;
int g_ResetEventCounter = 0;
int g_IsProcessorFeaturePresentCounter = 0;
int g_TerminateProcessCounter = 0;
int g_SetUnhandledExceptionFilterCounter = 0;
int g_UnhandledExceptionFilterCounter = 0;
int g_RtlVirtualUnwindCounter = 0;
int g_RtlLookupFunctionEntryCounter = 0;
int g_RtlCaptureContextCounter = 0;
int g_OutputDebugStringWCounter = 0;
int g_VirtualProtectCounter = 0;
int g_LoadLibraryExACounter = 0;

//int g_GetVolumeInformationWCounter = 0;
//int g_GetFinalPathNameByHandleWCounter = 0;
//int g_GetDriveTypeWCounter = 0;
//int g_FormatMessageWCounter = 0;
//int g_lstrcmpiWCounter = 0;
//int g_MultiByteToWideCharCounter = 0;
//int g_SizeofResourceCounter = 0;
//int g_GetCPInfoCounter = 0;
//int g_IsValidCodePageCounter = 0;
//int g_GetVersionExWCounter = 0;
//int g_OpenFileMappingWCounter = 0;
//int g_MapViewOfFileCounter = 0;
//int g_RaiseFailFastExceptionCounter = 0;
//int g_ExitProcessCounter = 0;
//int g_ReleaseSemaphoreCounter = 0;
//int g_CreateSemaphoreWCounter = 0;
//int g_GetFileSizeCounter = 0;
//int g_SetProcessWorkingSetSizeCounter = 0;
//int g_LCMapStringWCounter = 0;
//int g_GetLongPathNameWCounter = 0;
//int g_OpenProcessCounter = 0;
//int g_SetLastErrorCounter = 0;
//int g_LocalFreeCounter = 0;
//int g_LocalAllocCounter = 0;
//int g_LoadLibraryACounter = 0;
//int g_LockFileExCounter = 0;
//int g_UnlockFileExCounter = 0;
//int g_GetFileAttributesExWCounter = 0;
//int g_GetFileTimeCounter = 0;
//int g_SetFileTimeCounter = 0;

extern std::string FileNameFromMappedMem (LPVOID pMem);
extern std::string FileNameFromHandle (HANDLE fileHandle);

static void LogCall (int& counter, const std::string& log) {
	++counter;

	FILE* flog = fopen ("d:\\work\\call_timing.log", "ab");
	if (flog) {
		clock_t clk = clock ();
		fprintf (flog, "[%.2f sec] %s -> %s [%d]\n", (double) clk / (double) CLOCKS_PER_SEC, g_logModule.c_str (), log.c_str (), counter);
		fclose (flog);
	}
}

static std::string ConvertWide (const std::wstring& log) {
	std::vector<char> buffer (log.size ());
	std::wcstombs (&buffer[0], &log[0], log.size ());
	return std::string (buffer.begin (), buffer.end ());
}

extern "C" __declspec(dllexport) BOOL CreateDirectoryW (LPCWSTR path, void* sa) {
	LogCall (g_CreateDirectoryWCounter, "CreateDirectoryW -> path: " + ConvertWide (path == nullptr ? L"null" : path));

	if (!g_CreateDirectoryW) {
		return 0;
	}
	return g_CreateDirectoryW (path, sa);
}

extern "C" __declspec(dllexport) BOOL ReadFile (HANDLE hFile, LPVOID buffer, DWORD bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) {
	LogCall (g_ReadFileCounter, "ReadFile -> path: " + FileNameFromHandle (hFile));

	if (!g_ReadFile) {
		return 0;
	}
	return g_ReadFile (hFile, buffer, bytesToRead, bytesRead, overlapped);
}

extern "C" __declspec(dllexport) LPWSTR GetCommandLineW (void) {
	LogCall (g_GetCommandLineWCounter, "GetCommandLineW");

	if (!g_GetCommandLineW) {
		return nullptr;
	}
	LPWSTR cmdLine = g_GetCommandLineW ();
	if (cmdLine) {
		LogCall (--g_GetCommandLineWCounter, "GetCommandLineW -> cmd line: " + ConvertWide (cmdLine));
	}
	return cmdLine;
}

extern "C" __declspec(dllexport) HANDLE G_tCurrentProcess (void) {
	LogCall (g_GetCurrentProcessCounter, "GetCurrentProcess");

	if (!g_GetCurrentProcess) {
		return 0;
	}
	return g_GetCurrentProcess ();
}

extern "C" __declspec(dllexport) DWORD GetModuleFileNameW (HMODULE hModule, LPWSTR lpFileName, DWORD size) {
	LogCall (g_GetModuleFileNameWCounter, "GetModuleFileNameW");

	if (!g_GetModuleFileNameW) {
		return 0;
	}
	return g_GetModuleFileNameW (hModule, lpFileName, size);
}

extern "C" __declspec(dllexport) UINT SetErrorMode (UINT mode) {
	LogCall (g_SetErrorModeCounter, "SetErrorMode");

	if (!g_SetErrorMode) {
		return 0;
	}
	return g_SetErrorMode (mode);
}

extern "C" __declspec(dllexport) DWORD SetFilePointer (HANDLE hFile, LONG distance, LONG *highword, DWORD method) {
	LogCall (g_SetFilePointerCounter, "SetFilePointer -> path: " + FileNameFromHandle (hFile));

	if (!g_SetFilePointer) {
		return 0;
	}
	return g_SetFilePointer (hFile, distance, highword, method);
}

extern "C" __declspec(dllexport) DWORD GetEnvironmentVariableW (LPCWSTR name, LPWSTR val, DWORD size) {
	LogCall (g_GetEnvironmentVariableWCounter, "GetEnvironmentVariableW -> name: " + ConvertWide (name == nullptr ? L"null" : name));

	if (!g_GetEnvironmentVariableW) {
		return 0;
	}

	DWORD len = g_GetEnvironmentVariableW (name, val, size);
	if (len > 0) {
		LogCall (--g_GetEnvironmentVariableWCounter, "GetEnvironmentVariableW -> value: " + ConvertWide (val == nullptr ? L"null" : std::wstring (val, val + len)));
	}
	return len;
}

extern "C" __declspec(dllexport) HANDLE CreateMutexW (void* sa, BOOL owner, LPCWSTR name) {
	LogCall (g_CreateMutexWCounter, "CreateMutexW");

	if (!g_CreateMutexW) {
		return 0;
	}
	return g_CreateMutexW (sa, owner, name);
}

extern "C" __declspec(dllexport) BOOL InitializeCriticalSectionEx (void* crit, DWORD spincount, DWORD flags) {
	LogCall (g_InitializeCriticalSectionExCounter, "InitializeCriticalSectionEx");

	if (!g_InitializeCriticalSectionEx) {
		return 0;
	}
	return g_InitializeCriticalSectionEx (crit, spincount, flags);
}

extern "C" __declspec(dllexport) DWORD WaitForSingleObject (HANDLE handle, DWORD timeout) {
	LogCall (g_WaitForSingleObjectCounter, "WaitForSingleObject");

	if (!g_WaitForSingleObject) {
		return 0;
	}
	return g_WaitForSingleObject (handle, timeout);
}

extern "C" __declspec(dllexport) HANDLE CreateFileW (LPCWSTR filename, DWORD access, DWORD sharing, void* sa, DWORD creation, DWORD attributes, HANDLE templateParam) {
	std::string log = ConvertWide (std::wstring (L"CreateFileW -> name: ") + (filename == nullptr ? L"null" : filename));
	LogCall (g_CreateFileWCounter, log);

	if (!g_CreateFileW) {
		return 0;
	}
	
	HANDLE fileHandle = g_CreateFileW (filename, access, sharing, sa, creation, attributes, templateParam);
	LogCall (--g_CreateFileWCounter, "CreateFileW -> handle: " + std::to_string ((unsigned long long) fileHandle));
	return fileHandle;
}

extern "C" __declspec(dllexport) BOOL ReleaseMutex (HANDLE handle) {
	LogCall (g_ReleaseMutexCounter, "ReleaseMutex");

	if (!g_ReleaseMutex) {
		return 0;
	}
	return g_ReleaseMutex (handle);
}

extern "C" __declspec(dllexport) HANDLE CreateToolhelp32Snapshot (DWORD flags, DWORD process) {
	LogCall (g_CreateToolhelp32SnapshotCounter, "CreateToolhelp32Snapshot");

	if (!g_CreateToolhelp32Snapshot) {
		return 0;
	}
	return g_CreateToolhelp32Snapshot (flags, process);
}

extern "C" __declspec(dllexport) DWORD G_tLastError (void) {
	LogCall (g_GetLastErrorCounter, "GetLastError");

	if (!g_GetLastError) {
		return 0;
	}
	return g_GetLastError ();
}

extern "C" __declspec(dllexport) BOOL Process32NextW (HANDLE hSnapshot, void* lppe) {
	LogCall (g_Process32NextWCounter, "Process32NextW");

	if (!g_Process32NextW) {
		return 0;
	}
	return g_Process32NextW (hSnapshot, lppe);
}

extern "C" __declspec(dllexport) BOOL Process32FirstW (HANDLE hSnapshot, void* lppe) {
	LogCall (g_Process32FirstWCounter, "Process32FirstW");

	if (!g_Process32FirstW) {
		return 0;
	}
	return g_Process32FirstW (hSnapshot, lppe);
}

extern "C" __declspec(dllexport) BOOL CloseHandle (HANDLE handle) {
	LogCall (g_CloseHandleCounter, "CloseHandle");

	if (!g_CloseHandle) {
		return 0;
	}
	return g_CloseHandle (handle);
}

extern "C" __declspec(dllexport) void R_iseException (DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args) {
	LogCall (g_RaiseExceptionCounter, "RaiseException");

	if (!g_RaiseException) {
		return;
	}
	g_RaiseException (code, flags, nbargs, args);
}

extern "C" __declspec(dllexport) VOID GetSystemInfo (LPSYSTEM_INFO si) {
	LogCall (g_GetSystemInfoCounter, "GetSystemInfo");

	if (!g_GetSystemInfo) {
		return ;
	}
	g_GetSystemInfo (si);
}

extern "C" __declspec(dllexport) HMODULE LoadLibraryW (LPCWSTR libnameW) {
	LogCall (g_LoadLibraryWCounter, "LoadLibraryW -> libname: " + ConvertWide (libnameW == nullptr ? L"null" : libnameW));

	if (!g_LoadLibraryW) {
		return 0;
	}
	return g_LoadLibraryW (libnameW);
}

extern "C" __declspec(dllexport) BOOL SwitchToThread (void) {
	LogCall (g_SwitchToThreadCounter, "SwitchToThread");

	if (!g_SwitchToThread) {
		return 0;
	}
	return g_SwitchToThread ();
}

extern "C" __declspec(dllexport) void* DecodePointer (void * ptr) {
	LogCall (g_DecodePointerCounter, "DecodePointer");

	if (!g_DecodePointer) {
		return 0;
	}
	return g_DecodePointer (ptr);
}

extern "C" __declspec(dllexport) FARPROC G_tProcAddress (HMODULE hModule, LPCSTR function) {
	LogCall (g_GetProcAddressCounter, "GetProcAddress -> function: " + std::string (function == nullptr ? "null" : function));

	if (!g_GetProcAddress) {
		return 0;
	}
	return g_GetProcAddress (hModule, function);
}

extern "C" __declspec(dllexport) void DeleteCriticalSection (void* crit) {
	LogCall (g_DeleteCriticalSectionCounter, "DeleteCriticalSection");

	if (!g_DeleteCriticalSection) {
		return;
	}
	g_DeleteCriticalSection (crit);
}

extern "C" __declspec(dllexport) BOOL CreateProcessW (LPCWSTR app_name, LPWSTR cmd_line, void* process_attr, void* thread_attr, BOOL inherit, DWORD flags, LPVOID env, LPCWSTR cur_dir, LPSTARTUPINFOW startup_info, LPPROCESS_INFORMATION info) {
	std::string log = "CreateProcessW -> app: " + ConvertWide (app_name == nullptr ? L"null" : app_name);
	log += ", cmd line: " + ConvertWide (cmd_line == nullptr ? L"null" : cmd_line);
	log += ", cur dir: " + ConvertWide (cur_dir == nullptr ? L"null" : cur_dir);

	LogCall (g_CreateProcessWCounter, log);

	if (!g_CreateProcessW) {
		return 0;
	}
	return g_CreateProcessW (app_name, cmd_line, process_attr, thread_attr, inherit, flags, env, cur_dir, startup_info, info);
}

extern "C" __declspec(dllexport) HMODULE G_tModuleHandleW (LPCWSTR module) {
	LogCall (g_GetModuleHandleWCounter, "GetModuleHandleW");

	if (!g_GetModuleHandleW) {
		return 0;
	}
	return g_GetModuleHandleW (module);
}

extern "C" __declspec(dllexport) BOOL F_eeLibrary (HINSTANCE hLibModule) {
	LogCall (g_FreeLibraryCounter, "FreeLibrary");

	if (!g_FreeLibrary) {
		return 0;
	}
	return g_FreeLibrary (hLibModule);
}

extern "C" __declspec(dllexport) HANDLE OpenMutexW (DWORD access, BOOL inherit, LPCWSTR name) {
	LogCall (g_OpenMutexWCounter, "OpenMutexW");

	if (!g_OpenMutexW) {
		return 0;
	}
	return g_OpenMutexW (access, inherit, name);
}

extern "C" __declspec(dllexport) SIZE_T V_rtualQuery (LPCVOID addr, PMEMORY_BASIC_INFORMATION info, SIZE_T len) {
	LogCall (g_VirtualQueryCounter, "VirtualQuery");

	if (!g_VirtualQuery) {
		return 0;
	}
	return g_VirtualQuery (addr, info, len);
}

extern "C" __declspec(dllexport) HMODULE LoadLibraryExW (LPCWSTR libnameW, HANDLE hfile, DWORD flags) {
	LogCall (g_LoadLibraryExWCounter, "LoadLibraryExW -> libname: " + ConvertWide (libnameW == nullptr ? L"null" : libnameW) + ", path: " + FileNameFromHandle (hfile));

	if (!g_LoadLibraryExW) {
		return 0;
	}
	return g_LoadLibraryExW (libnameW, hfile, flags);
}

extern "C" __declspec(dllexport) DWORD GetTempPathW (DWORD count, LPWSTR path) {
	LogCall (g_GetTempPathWCounter, "GetTempPathW");

	if (!g_GetTempPathW) {
		return 0;
	}
	return g_GetTempPathW (count, path);
}

extern "C" __declspec(dllexport) BOOL GetDiskFreeSpaceExW (LPCWSTR root, PULARGE_INTEGER avail, PULARGE_INTEGER total, PULARGE_INTEGER totalfree) {
	LogCall (g_GetDiskFreeSpaceExWCounter, "GetDiskFreeSpaceExW");

	if (!g_GetDiskFreeSpaceExW) {
		return 0;
	}
	return g_GetDiskFreeSpaceExW (root, avail, total, totalfree);
}

extern "C" __declspec(dllexport) INT W_deCharToMultiByte (UINT page, DWORD flags, LPCWSTR src, INT srclen, LPSTR dst, INT dstlen, LPCSTR defchar, BOOL *used) {
	LogCall (g_WideCharToMultiByteCounter, "WideCharToMultiByte");

	if (!g_WideCharToMultiByte) {
		return 0;
	}
	return g_WideCharToMultiByte (page, flags, src, srclen, dst, dstlen, defchar, used);
}

extern "C" __declspec(dllexport) BOOL SetConsoleCtrlHandler (PHANDLER_ROUTINE func, BOOL add) {
	LogCall (g_SetConsoleCtrlHandlerCounter, "SetConsoleCtrlHandler");

	if (!g_SetConsoleCtrlHandler) {
		return 0;
	}
	return g_SetConsoleCtrlHandler (func, add);
}

extern "C" __declspec(dllexport) BOOL GetConsoleScreenBufferInfo (HANDLE hConsoleOutput, void* csbi) {
	LogCall (g_GetConsoleScreenBufferInfoCounter, "GetConsoleScreenBufferInfo");

	if (!g_GetConsoleScreenBufferInfo) {
		return 0;
	}
	return g_GetConsoleScreenBufferInfo (hConsoleOutput, csbi);
}

extern "C" __declspec(dllexport) HANDLE OpenEventW (DWORD access, BOOL inherit, LPCWSTR name) {
	LogCall (g_OpenEventWCounter, "OpenEventW");

	if (!g_OpenEventW) {
		return 0;
	}
	return g_OpenEventW (access, inherit, name);
}

extern "C" __declspec(dllexport) BOOL SetEvent (HANDLE handle) {
	LogCall (g_SetEventCounter, "SetEvent");

	if (!g_SetEvent) {
		return 0;
	}
	return g_SetEvent (handle);
}

extern "C" __declspec(dllexport) UINT GetConsoleOutputCP (VOID) {
	LogCall (g_GetConsoleOutputCPCounter, "GetConsoleOutputCP");

	if (!g_GetConsoleOutputCP) {
		return 0;
	}
	return g_GetConsoleOutputCP ();
}

extern "C" __declspec(dllexport) BOOL WriteFile (HANDLE hFile, LPCVOID buffer, DWORD bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) {
	LogCall (g_WriteFileCounter, "WriteFile -> path: " + FileNameFromHandle (hFile));

	if (!g_WriteFile) {
		return 0;
	}
	return g_WriteFile (hFile, buffer, bytesToWrite, bytesWritten, overlapped);
}

extern "C" __declspec(dllexport) UINT GetACP (void) {
	LogCall (g_GetACPCounter, "GetACP");

	if (!g_GetACP) {
		return 0;
	}
	return g_GetACP ();
}

extern "C" __declspec(dllexport) BOOL GetConsoleMode (HANDLE hcon, LPDWORD mode) {
	LogCall (g_GetConsoleModeCounter, "GetConsoleMode");

	if (!g_GetConsoleMode) {
		return 0;
	}
	return g_GetConsoleMode (hcon, mode);
}

extern "C" __declspec(dllexport) BOOL QueryPerformanceFrequency (PLARGE_INTEGER frequency) {
	LogCall (g_QueryPerformanceFrequencyCounter, "QueryPerformanceFrequency");

	if (!g_QueryPerformanceFrequency) {
		return 0;
	}
	return g_QueryPerformanceFrequency (frequency);
}

extern "C" __declspec(dllexport) HGLOBAL LoadResource (HINSTANCE hModule, HRSRC hRsrc) {
	LogCall (g_LoadResourceCounter, "LoadResource");

	if (!g_LoadResource) {
		return 0;
	}
	return g_LoadResource (hModule, hRsrc);
}

extern "C" __declspec(dllexport) HRSRC FindResourceW (HINSTANCE hModule, LPCWSTR name, LPCWSTR type) {
	LogCall (g_FindResourceWCounter, "FindResourceW");

	if (!g_FindResourceW) {
		return 0;
	}
	return g_FindResourceW (hModule, name, type);
}

extern "C" __declspec(dllexport) DWORD GetFileType (HANDLE hFile) {
	LogCall (g_GetFileTypeCounter, "GetFileType -> path: " + FileNameFromHandle (hFile));

	if (!g_GetFileType) {
		return 0;
	}
	return g_GetFileType (hFile);
}

extern "C" __declspec(dllexport) BOOL Q_eryPerformanceCounter (PLARGE_INTEGER counter) {
	LogCall (g_QueryPerformanceCounterCounter, "QueryPerformanceCounter");

	if (!g_QueryPerformanceCounter) {
		return 0;
	}
	return g_QueryPerformanceCounter (counter);
}

extern "C" __declspec(dllexport) BOOL VirtualFree (LPVOID addr, SIZE_T size, DWORD type) {
	LogCall (g_VirtualFreeCounter, "VirtualFree");

	if (!g_VirtualFree) {
		return 0;
	}
	return g_VirtualFree (addr, size, type);
}

extern "C" __declspec(dllexport) LPVOID VirtualAlloc (void *addr, SIZE_T size, DWORD type, DWORD protect) {
	LogCall (g_VirtualAllocCounter, "VirtualAlloc");

	if (!g_VirtualAlloc) {
		return 0;
	}
	return g_VirtualAlloc (addr, size, type, protect);
}

extern "C" __declspec(dllexport) BOOL UnmapViewOfFile (LPCVOID addr) {
	LogCall (g_UnmapViewOfFileCounter, "UnmapViewOfFile");

	if (!g_UnmapViewOfFile) {
		return 0;
	}
	return g_UnmapViewOfFile (addr);
}

extern "C" __declspec(dllexport) HANDLE CreateFileMappingW (HANDLE hFile, void* sa, DWORD protect, DWORD size_high, DWORD size_low, LPCWSTR name) {
	LogCall (g_CreateFileMappingWCounter, "CreateFileMappingW -> path: " + FileNameFromHandle (hFile));

	if (!g_CreateFileMappingW) {
		return 0;
	}
	return g_CreateFileMappingW (hFile, sa, protect, size_high, size_low, name);
}

extern "C" __declspec(dllexport) LPVOID MapViewOfFileEx (HANDLE handle, DWORD access, DWORD offset_high, DWORD offset_low, SIZE_T count, LPVOID addr) {
	LogCall (g_MapViewOfFileExCounter, "MapViewOfFileEx-> path: " + FileNameFromHandle (handle));

	if (!g_MapViewOfFileEx) {
		return 0;
	}
	
	LPVOID pMem =  g_MapViewOfFileEx (handle, access, offset_high, offset_low, count, addr);
	if (pMem) {
		std::string fileName = FileNameFromMappedMem (pMem);
		LogCall (--g_MapViewOfFileExCounter, "MapViewOfFileEx -> fileName: " + fileName);
	}
	return pMem;
}

extern "C" __declspec(dllexport) VOID G_tStartupInfoW (LPSTARTUPINFOW info) {
	LogCall (g_GetStartupInfoWCounter, "GetStartupInfoW");

	if (!g_GetStartupInfoW) {
		return;
	}
	g_GetStartupInfoW (info);
}

extern "C" __declspec(dllexport) NTSTATUS EnterCriticalSection (void* crit) {
	LogCall (g_EnterCriticalSectionCounter, "EnterCriticalSection");

	if (!g_EnterCriticalSection) {
		return 0;
	}
	return g_EnterCriticalSection (crit);
}

extern "C" __declspec(dllexport) HANDLE GetStdHandle (DWORD std_handle) {
	LogCall (g_GetStdHandleCounter, "GetStdHandle");

	if (!g_GetStdHandle) {
		return 0;
	}
	return g_GetStdHandle (std_handle);
}

extern "C" __declspec(dllexport) DWORD WaitForMultipleObjects (DWORD count, const HANDLE *handles, BOOL wait_all, DWORD timeout) {
	LogCall (g_WaitForMultipleObjectsCounter, "WaitForMultipleObjects");

	if (!g_WaitForMultipleObjects) {
		return 0;
	}
	return g_WaitForMultipleObjects (count, handles, wait_all, timeout);
}

extern "C" __declspec(dllexport) BOOL SetEnvironmentVariableW (LPCWSTR name, LPCWSTR value) {
	LogCall (g_SetEnvironmentVariableWCounter, "SetEnvironmentVariableW");

	if (!g_SetEnvironmentVariableW) {
		return 0;
	}
	return g_SetEnvironmentVariableW (name, value);
}

extern "C" __declspec(dllexport) BOOL SetThreadPriority (HANDLE hthread, INT priority) {
	LogCall (g_SetThreadPriorityCounter, "SetThreadPriority");

	if (!g_SetThreadPriority) {
		return 0;
	}
	return g_SetThreadPriority (hthread, priority);
}

extern "C" __declspec(dllexport) NTSTATUS LeaveCriticalSection (void* crit) {
	LogCall (g_LeaveCriticalSectionCounter, "LeaveCriticalSection");

	if (!g_LeaveCriticalSection) {
		return 0;
	}
	return g_LeaveCriticalSection (crit);
}

extern "C" __declspec(dllexport) BOOL CreatePipe (PHANDLE hReadPipe, PHANDLE hWritePipe, void* sa, DWORD size) {
	LogCall (g_CreatePipeCounter, "CreatePipe");

	if (!g_CreatePipe) {
		return 0;
	}
	return g_CreatePipe (hReadPipe, hWritePipe, sa, size);
}

extern "C" __declspec(dllexport) void InitializeCriticalSection (void* crit) {
	LogCall (g_InitializeCriticalSectionCounter, "InitializeCriticalSection");

	if (!g_InitializeCriticalSection) {
		return;
	}
	g_InitializeCriticalSection (crit);
}

extern "C" __declspec(dllexport) BOOL DuplicateHandle (HANDLE source_process, HANDLE source, HANDLE dest_process, HANDLE *dest, DWORD access, BOOL inherit, DWORD options) {
	LogCall (g_DuplicateHandleCounter, "DuplicateHandle");

	if (!g_DuplicateHandle) {
		return 0;
	}
	return g_DuplicateHandle (source_process, source, dest_process, dest, access, inherit, options);
}

extern "C" __declspec(dllexport) VOID Sleep (DWORD timeout) {
	LogCall (g_SleepCounter, "Sleep");

	if (!g_Sleep) {
		return;
	}
	g_Sleep (timeout);
}

extern "C" __declspec(dllexport) HANDLE CreateThread (void* sa, SIZE_T stack, LPTHREAD_START_ROUTINE start, LPVOID param, DWORD flags, LPDWORD id) {
	LogCall (g_CreateThreadCounter, "CreateThread");

	if (!g_CreateThread) {
		return 0;
	}
	return g_CreateThread (sa, stack, start, param, flags, id);
}

extern "C" __declspec(dllexport) DWORD G_tCurrentProcessId (void) {
	LogCall (g_GetCurrentProcessIdCounter, "GetCurrentProcessId");

	if (!g_GetCurrentProcessId) {
		return 0;
	}
	return g_GetCurrentProcessId ();
}

extern "C" __declspec(dllexport) LPWSTR GetEnvironmentStringsW (void) {
	LogCall (g_GetEnvironmentStringsWCounter, "GetEnvironmentStringsW");

	if (!g_GetEnvironmentStringsW) {
		return 0;
	}
	return g_GetEnvironmentStringsW ();
}

extern "C" __declspec(dllexport) HANDLE FindFirstFileW (LPCWSTR lpFileName, void* lpFindData) {
	std::string log = ConvertWide (std::wstring (L"FindFirstFileW -> name: ") + (lpFileName == nullptr ? L"null" : lpFileName));
	LogCall (g_FindFirstFileWCounter, log);

	if (!g_FindFirstFileW) {
		return 0;
	}

	struct FF_WIN32_FIND_DATA {
		DWORD    dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD    nFileSizeHigh;
		DWORD    nFileSizeLow;
		DWORD    dwReserved0;
		DWORD    dwReserved1;
		WCHAR    cFileName[MAX_PATH];
		WCHAR    cAlternateFileName[14];
	} *findData = (FF_WIN32_FIND_DATA*) lpFindData;

	HANDLE fileHandle = g_FindFirstFileW (lpFileName, lpFindData);

	log = "FindFirstFileW -> handle: " + std::to_string ((unsigned long long) fileHandle);
	if (findData) {
		log += ", name: " + ConvertWide (findData->cFileName == nullptr ? L"null" : findData->cFileName);
	}
	LogCall (--g_FindFirstFileWCounter, log);

	return fileHandle;
}

extern "C" __declspec(dllexport) BOOLEAN H_apFree (HANDLE heap, ULONG flags, PVOID ptr) {
	LogCall (g_HeapFreeCounter, "HeapFree");

	if (!g_HeapFree) {
		return 0;
	}
	return g_HeapFree (heap, flags, ptr);
}

extern "C" __declspec(dllexport) BOOL FindNextFileW (HANDLE handle, void* data) {
	LogCall (g_FindNextFileWCounter, "FindNextFileW");

	if (!g_FindNextFileW) {
		return 0;
	}

	struct FF_WIN32_FIND_DATA {
		DWORD    dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD    nFileSizeHigh;
		DWORD    nFileSizeLow;
		DWORD    dwReserved0;
		DWORD    dwReserved1;
		WCHAR    cFileName[MAX_PATH];
		WCHAR    cAlternateFileName[14];
	} *findData = (FF_WIN32_FIND_DATA*) data;

	BOOL res = g_FindNextFileW (handle, data);
	if (findData) {
		std::string log = "FindNextFileW -> name: " + ConvertWide (findData->cFileName == nullptr ? L"null" : findData->cFileName);
		LogCall (--g_FindNextFileWCounter, log);
	}
	return res;
}

extern "C" __declspec(dllexport) LANGID GetUserDefaultUILanguage (void) {
	LogCall (g_GetUserDefaultUILanguageCounter, "GetUserDefaultUILanguage");

	if (!g_GetUserDefaultUILanguage) {
		return 0;
	}
	return g_GetUserDefaultUILanguage ();
}

extern "C" __declspec(dllexport) PVOID EncodePointer (PVOID ptr) {
	LogCall (g_EncodePointerCounter, "EncodePointer");

	if (!g_EncodePointer) {
		return 0;
	}
	return g_EncodePointer (ptr);
}

extern "C" __declspec(dllexport) BOOL FindClose (HANDLE handle) {
	LogCall (g_FindCloseCounter, "FindClose");

	if (!g_FindClose) {
		return 0;
	}
	return g_FindClose (handle);
}

extern "C" __declspec(dllexport) PVOID H_apAlloc (HANDLE heap, ULONG flags, SIZE_T size) {
	LogCall (g_HeapAllocCounter, "HeapAlloc");

	if (!g_HeapAlloc) {
		return FALSE;
	}
	return g_HeapAlloc (heap, flags, size);
}

extern "C" __declspec(dllexport) HANDLE G_tProcessHeap (void) {
	LogCall (g_GetProcessHeapCounter, "GetProcessHeap");

	if (!g_GetProcessHeap) {
		return FALSE;
	}
	return g_GetProcessHeap ();
}

extern "C" __declspec(dllexport) DWORD ExpandEnvironmentStringsW (LPCWSTR src, LPWSTR dst, DWORD count) {
	LogCall (g_ExpandEnvironmentStringsWCounter, "ExpandEnvironmentStringsW");

	if (!g_ExpandEnvironmentStringsW) {
		return FALSE;
	}
	return g_ExpandEnvironmentStringsW (src, dst, count);
}

extern "C" __declspec(dllexport) DWORD G_tCurrentThreadId (void) {
	LogCall (g_GetCurrentThreadIdCounter, "GetCurrentThreadId");

	if (!g_GetCurrentThreadId) {
		return FALSE;
	}
	return g_GetCurrentThreadId ();
}

extern "C" __declspec(dllexport) DWORD GetVersion (void) {
	LogCall (g_GetVersionCounter, "GetVersion");

	if (!g_GetVersion) {
		return FALSE;
	}
	return g_GetVersion ();
}

extern "C" __declspec(dllexport) HANDLE CreateEventW (void* sa, BOOL manual_reset, BOOL initial_state, LPCWSTR name) {
	LogCall (g_CreateEventWCounter, "CreateEventW");

	if (!g_CreateEventW) {
		return FALSE;
	}
	return g_CreateEventW (sa, manual_reset, initial_state, name);
}

extern "C" __declspec(dllexport) BOOL I_DebuggerPresent (void) {
	LogCall (g_IsDebuggerPresentCounter, "IsDebuggerPresent");

	if (!g_IsDebuggerPresent) {
		return FALSE;
	}
	return g_IsDebuggerPresent ();
}

extern "C" __declspec(dllexport) VOID I_itializeSListHead (PSLIST_HEADER list) {
	LogCall (g_InitializeSListHeadCounter, "InitializeSListHead");

	if (!g_InitializeSListHead) {
		return;
	}
	g_InitializeSListHead (list);
}

extern "C" __declspec(dllexport) VOID G_tSystemTimeAsFileTime (LPFILETIME time) {
	LogCall (g_GetSystemTimeAsFileTimeCounter, "GetSystemTimeAsFileTime");

	if (!g_GetSystemTimeAsFileTime) {
		return;
	}
	g_GetSystemTimeAsFileTime (time);
}

extern "C" __declspec(dllexport) DWORD WaitForSingleObjectEx (HANDLE handle, DWORD timeout, BOOL alertable) {
	LogCall (g_WaitForSingleObjectExCounter, "WaitForSingleObjectEx");

	if (!g_WaitForSingleObjectEx) {
		return FALSE;
	}
	return g_WaitForSingleObjectEx (handle, timeout, alertable);
}

extern "C" __declspec(dllexport) BOOL ResetEvent (HANDLE handle) {
	LogCall (g_ResetEventCounter, "ResetEvent");

	if (!g_ResetEvent) {
		return FALSE;
	}
	return g_ResetEvent (handle);
}

extern "C" __declspec(dllexport) BOOL IsProcessorFeaturePresent (DWORD feature) {
	LogCall (g_IsProcessorFeaturePresentCounter, "IsProcessorFeaturePresent");

	if (!g_IsProcessorFeaturePresent) {
		return FALSE;
	}
	return g_IsProcessorFeaturePresent (feature);
}

extern "C" __declspec(dllexport) BOOL T_rminateProcess (HANDLE handle, DWORD exit_code) {
	LogCall (g_TerminateProcessCounter, "TerminateProcess");

	if (!g_TerminateProcess) {
		return FALSE;
	}
	return g_TerminateProcess (handle, exit_code);
}

extern "C" __declspec(dllexport) LPTOP_LEVEL_EXCEPTION_FILTER S_tUnhandledExceptionFilter (LPTOP_LEVEL_EXCEPTION_FILTER filter) {
	LogCall (g_SetUnhandledExceptionFilterCounter, "SetUnhandledExceptionFilter");

	if (!g_SetUnhandledExceptionFilter) {
		return FALSE;
	}
	return g_SetUnhandledExceptionFilter (filter);
}

extern "C" __declspec(dllexport) LONG U_handledExceptionFilter (PEXCEPTION_POINTERS epointers) {
	LogCall (g_UnhandledExceptionFilterCounter, "UnhandledExceptionFilter");

	if (!g_UnhandledExceptionFilter) {
		return FALSE;
	}
	return g_UnhandledExceptionFilter (epointers);
}

extern "C" __declspec(dllexport) PVOID R_lVirtualUnwind (ULONG type, ULONG64 base, ULONG64 pc, void* function, void* context, PVOID *data, ULONG64 *frame_ret, void* ctx_ptr) {
	LogCall (g_RtlVirtualUnwindCounter, "RtlVirtualUnwind");

	if (!g_RtlVirtualUnwind) {
		return FALSE;
	}
	return g_RtlVirtualUnwind (type, base, pc, function, context, data, frame_ret, ctx_ptr);
}

extern "C" __declspec(dllexport) PRUNTIME_FUNCTION R_lLookupFunctionEntry (ULONG64 pc, ULONG64 *base, void* table) {
	LogCall (g_RtlLookupFunctionEntryCounter, "RtlLookupFunctionEntry");

	if (!g_RtlLookupFunctionEntry) {
		return FALSE;
	}
	return g_RtlLookupFunctionEntry (pc, base, table);
}

extern "C" __declspec(dllexport) VOID R_lCaptureContext (VOID* ptr) {
	LogCall (g_RtlCaptureContextCounter, "RtlCaptureContext");

	if (!g_RtlCaptureContext) {
		return;
	}
	g_RtlCaptureContext (ptr);
}

extern "C" __declspec(dllexport) void OutputDebugStringW (LPCWSTR str) {
	LogCall (g_OutputDebugStringWCounter, "OutputDebugStringW -> str: " + ConvertWide(str == nullptr ? L"null" : str));

	if (!g_OutputDebugStringW) {
		return;
	}
	g_OutputDebugStringW (str);
}

extern "C" __declspec(dllexport) BOOL VirtualProtect (LPVOID addr, SIZE_T size, DWORD new_prot, LPDWORD old_prot) {
	LogCall (g_VirtualProtectCounter, "VirtualProtect");

	if (!g_VirtualProtect) {
		return FALSE;
	}
	return g_VirtualProtect (addr, size, new_prot, old_prot);
}

extern "C" __declspec(dllexport) HMODULE LoadLibraryExA (LPCSTR libname, HANDLE hfile, DWORD flags) {
	LogCall (g_LoadLibraryExACounter, "LoadLibraryExA -> libname: " + std::string (libname == nullptr ? "null" : libname) + ", path: " + FileNameFromHandle (hfile));

	if (!g_LoadLibraryExA) {
		return FALSE;
	}
	return g_LoadLibraryExA (libname, hfile, flags);
}





//extern "C" __declspec(dllexport) BOOL GetVolumeInformationW (LPCWSTR root, LPWSTR label, DWORD label_len, DWORD *serial, DWORD *filename_len, DWORD *flags, LPWSTR fsname, DWORD fsname_len) {
//	LogCall (g_GetVolumeInformationWCounter, "GetVolumeInformationW");
//
//	if (!g_GetVolumeInformationW) {
//		return 0;
//	}
//	return g_GetVolumeInformationW (root, label, label_len, serial, filename_len, flags, fsname, fsname_len);
//}
//
//extern "C" __declspec(dllexport) DWORD GetFinalPathNameByHandleW (HANDLE file, LPWSTR path, DWORD charcount, DWORD flags) {
//	LogCall (g_GetFinalPathNameByHandleWCounter, "GetFinalPathNameByHandleW");
//
//	if (!g_GetFinalPathNameByHandleW) {
//		return 0;
//	}
//	return g_GetFinalPathNameByHandleW (file, path, charcount, flags);
//}
//
//extern "C" __declspec(dllexport) UINT GetDriveTypeW (LPCWSTR root) {
//	LogCall (g_GetDriveTypeWCounter, "GetDriveTypeW");
//
//	if (!g_GetDriveTypeW) {
//		return 0;
//	}
//	return g_GetDriveTypeW (root);
//}
//
//extern "C" __declspec(dllexport) DWORD FormatMessageW (DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPWSTR lpBuffer, DWORD nSize, void* args) {
//	LogCall (g_FormatMessageWCounter, "FormatMessageW");
//
//	if (!g_FormatMessageW) {
//		return 0;
//	}
//	return g_FormatMessageW (dwFlags, lpSource, dwMessageId, dwLanguageId, lpBuffer, nSize, args);
//}
//
//extern "C" __declspec(dllexport) int lstrcmpiW (LPCWSTR str1, LPCWSTR str2) {
//	LogCall (g_lstrcmpiWCounter, "lstrcmpiW");
//
//	if (!g_lstrcmpiW) {
//		return 0;
//	}
//	return g_lstrcmpiW (str1, str2);
//}
//
//extern "C" __declspec(dllexport) INT M_ltiByteToWideChar (UINT page, DWORD flags, LPCSTR src, INT srclen, LPWSTR dst, INT dstlen) {
//	LogCall (g_MultiByteToWideCharCounter, "MultiByteToWideChar");
//
//	if (!g_MultiByteToWideChar) {
//		return 0;
//	}
//	return g_MultiByteToWideChar (page, flags, src, srclen, dst, dstlen);
//}
//
//extern "C" __declspec(dllexport) DWORD SizeofResource (HINSTANCE hModule, HRSRC hRsrc) {
//	LogCall (g_SizeofResourceCounter, "SizeofResource");
//
//	if (!g_SizeofResource) {
//		return 0;
//	}
//	return g_SizeofResource (hModule, hRsrc);
//}
//
//extern "C" __declspec(dllexport) BOOL GetCPInfo (UINT codepage, LPCPINFO cpinfo) {
//	LogCall (g_GetCPInfoCounter, "GetCPInfo");
//
//	if (!g_GetCPInfo) {
//		return 0;
//	}
//	return g_GetCPInfo (codepage, cpinfo);
//}
//
//extern "C" __declspec(dllexport) BOOL IsValidCodePage (UINT codepage) {
//	LogCall (g_IsValidCodePageCounter, "IsValidCodePage");
//
//	if (!g_IsValidCodePage) {
//		return 0;
//	}
//	return g_IsValidCodePage (codepage);
//}
//
//extern "C" __declspec(dllexport) BOOL GetVersionExW (void* info) {
//	LogCall (g_GetVersionExWCounter, "GetVersionExW");
//
//	if (!g_GetVersionExW) {
//		return 0;
//	}
//	return g_GetVersionExW (info);
//}
//
//extern "C" __declspec(dllexport) HANDLE OpenFileMappingW (DWORD access, BOOL inherit, LPCWSTR name) {
//	LogCall (g_OpenFileMappingWCounter, "OpenFileMappingW");
//
//	if (!g_OpenFileMappingW) {
//		return 0;
//	}
//	return g_OpenFileMappingW (access, inherit, name);
//}
//
//extern "C" __declspec(dllexport) LPVOID MapViewOfFile (HANDLE mapping, DWORD access, DWORD offset_high, DWORD offset_low, SIZE_T count) {
//	LogCall (g_MapViewOfFileCounter, "MapViewOfFile");
//
//	if (!g_MapViewOfFile) {
//		return 0;
//	}
//	return g_MapViewOfFile (mapping, access, offset_high, offset_low, count);
//}
//
//extern "C" __declspec(dllexport) void RaiseFailFastException (DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args) {
//	LogCall (g_RaiseFailFastExceptionCounter, "RaiseFailFastException");
//
//	if (!g_RaiseFailFastException) {
//		return;
//	}
//	g_RaiseFailFastException (code, flags, nbargs, args);
//}
//
//extern "C" __declspec(dllexport) void ExitProcess (DWORD status) {
//	LogCall (g_ExitProcessCounter, "ExitProcess");
//
//	if (!g_ExitProcess) {
//		return;
//	}
//	g_ExitProcess (status);
//}
//
//extern "C" __declspec(dllexport) BOOL ReleaseSemaphore (HANDLE handle, LONG count, LONG *previous) {
//	LogCall (g_ReleaseSemaphoreCounter, "ReleaseSemaphore");
//
//	if (!g_ReleaseSemaphore) {
//		return 0;
//	}
//	return g_ReleaseSemaphore (handle, count, previous);
//}
//
//extern "C" __declspec(dllexport) HANDLE CreateSemaphoreW (void* sa, LONG initial, LONG max, LPCWSTR name) {
//	LogCall (g_CreateSemaphoreWCounter, "CreateSemaphoreW");
//
//	if (!g_CreateSemaphoreW) {
//		return 0;
//	}
//	return g_CreateSemaphoreW (sa, initial, max, name);
//}
//
//extern "C" __declspec(dllexport) DWORD GetFileSize (HANDLE hFile, LPDWORD filesizehigh) {
//	LogCall (g_GetFileSizeCounter, "GetFileSize");
//
//	if (!g_GetFileSize) {
//		return 0;
//	}
//	return g_GetFileSize (hFile, filesizehigh);
//}
//
//extern "C" __declspec(dllexport) BOOL SetProcessWorkingSetSize (HANDLE hProcess, SIZE_T minset, SIZE_T maxset) {
//	LogCall (g_SetProcessWorkingSetSizeCounter, "SetProcessWorkingSetSize");
//
//	if (!g_SetProcessWorkingSetSize) {
//		return 0;
//	}
//	return g_SetProcessWorkingSetSize (hProcess, minset, maxset);
//}
//
//extern "C" __declspec(dllexport) INT LCMapStringW (LCID lcid, DWORD flags, LPCWSTR src, INT srclen, LPWSTR dst, INT dstlen) {
//	LogCall (g_LCMapStringWCounter, "LCMapStringW");
//
//	if (!g_LCMapStringW) {
//		return 0;
//	}
//	return g_LCMapStringW (lcid, flags, src, srclen, dst, dstlen);
//}
//
//extern "C" __declspec(dllexport) DWORD GetLongPathNameW (LPCWSTR shortpath, LPWSTR longpath, DWORD longlen) {
//	LogCall (g_GetLongPathNameWCounter, "GetLongPathNameW");
//
//	if (!g_GetLongPathNameW) {
//		return 0;
//	}
//	return g_GetLongPathNameW (shortpath, longpath, longlen);
//}
//
//extern "C" __declspec(dllexport) HANDLE OpenProcess (DWORD access, BOOL inherit, DWORD id) {
//	LogCall (g_OpenProcessCounter, "OpenProcess");
//
//	if (!g_OpenProcess) {
//		return 0;
//	}
//	return g_OpenProcess (access, inherit, id);
//}
//
//extern "C" __declspec(dllexport) void SetLastError (DWORD error) {
//	LogCall (g_SetLastErrorCounter, "SetLastError");
//
//	if (!g_SetLastError) {
//		return;
//	}
//	g_SetLastError (error);
//}
//
//extern "C" __declspec(dllexport) HLOCAL LocalFree (HLOCAL handle) {
//	LogCall (g_LocalFreeCounter, "LocalFree");
//
//	if (!g_LocalFree) {
//		return 0;
//	}
//	return g_LocalFree (handle);
//}
//
//extern "C" __declspec(dllexport) HLOCAL LocalAlloc (UINT flags, SIZE_T size) {
//	LogCall (g_LocalAllocCounter, "LocalAlloc");
//
//	if (!g_LocalAlloc) {
//		return 0;
//	}
//	return g_LocalAlloc (flags, size);
//}
//
//extern "C" __declspec(dllexport) HMODULE L_adLibraryA (LPCSTR libname) {
//	LogCall (g_LoadLibraryACounter, "LoadLibraryA");
//
//	if (!g_LoadLibraryA) {
//		return 0;
//	}
//	return g_LoadLibraryA (libname);
//}
//
//extern "C" __declspec(dllexport) BOOL LockFileEx (HANDLE hFile, DWORD flags, DWORD reserved, DWORD count_low, DWORD count_high, LPOVERLAPPED overlapped) {
//	LogCall (g_LockFileExCounter, "LockFileEx");
//
//	if (!g_LockFileEx) {
//		return 0;
//	}
//	return g_LockFileEx (hFile, flags, reserved, count_low, count_high, overlapped);
//}
//
//extern "C" __declspec(dllexport) BOOL UnlockFileEx (HANDLE hFile, DWORD reserved, DWORD count_low, DWORD count_high, LPOVERLAPPED overlapped) {
//	LogCall (g_UnlockFileExCounter, "UnlockFileEx");
//
//	if (!g_UnlockFileEx) {
//		return 0;
//	}
//	return g_UnlockFileEx (hFile, reserved, count_low, count_high, overlapped);
//}
//
//extern "C" __declspec(dllexport) BOOL GetFileAttributesExW (LPCWSTR name, GET_FILEEX_INFO_LEVELS level, LPVOID ptr) {
//	LogCall (g_GetFileAttributesExWCounter, "GetFileAttributesExW");
//
//	if (!g_GetFileAttributesExW) {
//		return 0;
//	}
//	return g_GetFileAttributesExW (name, level, ptr);
//}
//
//extern "C" __declspec(dllexport) BOOL GetFileTime (HANDLE hFile, FILETIME *lpCreationTime, FILETIME *lpLastAccessTime, FILETIME *lpLastWriteTime) {
//	LogCall (g_GetFileTimeCounter, "GetFileTime");
//
//	if (!g_GetFileTime) {
//		return 0;
//	}
//	return g_GetFileTime (hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
//}
//
//extern "C" __declspec(dllexport) BOOL SetFileTime (HANDLE hFile, const FILETIME *ctime, const FILETIME *atime, const FILETIME *mtime) {
//	LogCall (g_SetFileTimeCounter, "SetFileTime");
//
//	if (!g_SetFileTime) {
//		return 0;
//	}
//	return g_SetFileTime (hFile, ctime, atime, mtime);
//}
