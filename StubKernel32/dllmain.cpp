// dllmain.cpp : Defines the entry point for the DLL application.

#include <windows.h>
#include <Psapi.h>
#include <iostream>
#include <ostream>
#include <string>
//#include "defs.h"

HMODULE g_moduleHandle = nullptr;
std::string g_logModule;

extern "C" BOOL (*g_CreateDirectoryW) (LPCWSTR path, void* sa) = nullptr;
extern "C" BOOL (*g_ReadFile) (HANDLE hFile, LPVOID buffer, DWORD bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) = nullptr;
extern "C" LPWSTR (*g_GetCommandLineW) (void) = nullptr;
extern "C" HANDLE (*g_GetCurrentProcess) (void) = nullptr;
extern "C" DWORD (*g_GetModuleFileNameW) (HMODULE hModule, LPWSTR lpFileName, DWORD size) = nullptr;
extern "C" UINT (*g_SetErrorMode) (UINT mode) = nullptr;
extern "C" DWORD (*g_SetFilePointer) (HANDLE hFile, LONG distance, LONG *highword, DWORD method) = nullptr;
extern "C" DWORD (*g_GetEnvironmentVariableW) (LPCWSTR name, LPWSTR val, DWORD size) = nullptr;
extern "C" HANDLE (*g_CreateMutexW) (void* sa, BOOL owner, LPCWSTR name) = nullptr;
extern "C" BOOL (*g_InitializeCriticalSectionEx) (void* crit, DWORD spincount, DWORD flags) = nullptr;
extern "C" DWORD (*g_WaitForSingleObject) (HANDLE handle, DWORD timeout) = nullptr;
extern "C" HANDLE (*g_CreateFileW) (LPCWSTR filename, DWORD access, DWORD sharing, void* sa, DWORD creation, DWORD attributes, HANDLE templateParam) = nullptr;
extern "C" BOOL (*g_ReleaseMutex) (HANDLE handle) = nullptr;
extern "C" HANDLE (*g_CreateToolhelp32Snapshot) (DWORD flags, DWORD process) = nullptr;
extern "C" DWORD (*g_GetLastError) (void) = nullptr;
extern "C" BOOL (*g_Process32NextW) (HANDLE hSnapshot, void* lppe) = nullptr;
extern "C" BOOL (*g_Process32FirstW) (HANDLE hSnapshot, void* lppe) = nullptr;
extern "C" BOOL (*g_CloseHandle) (HANDLE handle) = nullptr;
extern "C" void (*g_RaiseException) (DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args) = nullptr;
extern "C" VOID (*g_GetSystemInfo) (LPSYSTEM_INFO si) = nullptr;
extern "C" HMODULE (*g_LoadLibraryW) (LPCWSTR libnameW) = nullptr;
extern "C" BOOL (*g_SwitchToThread) (void) = nullptr;
extern "C" void* (*g_DecodePointer) (void * ptr) = nullptr;
extern "C" FARPROC (*g_GetProcAddress) (HMODULE hModule, LPCSTR function) = nullptr;
extern "C" void (*g_DeleteCriticalSection) (void* crit) = nullptr;
extern "C" BOOL (*g_CreateProcessW) (LPCWSTR app_name, LPWSTR cmd_line, void* process_attr, void* thread_attr, BOOL inherit, DWORD flags, LPVOID env, LPCWSTR cur_dir, LPSTARTUPINFOW startup_info, LPPROCESS_INFORMATION info) = nullptr;
extern "C" HMODULE (*g_GetModuleHandleW) (LPCWSTR module) = nullptr;
extern "C" BOOL (*g_FreeLibrary) (HINSTANCE hLibModule) = nullptr;
extern "C" HANDLE (*g_OpenMutexW) (DWORD access, BOOL inherit, LPCWSTR name) = nullptr;
extern "C" SIZE_T (*g_VirtualQuery) (LPCVOID addr, PMEMORY_BASIC_INFORMATION info, SIZE_T len) = nullptr;
extern "C" HMODULE (*g_LoadLibraryExW) (LPCWSTR libnameW, HANDLE hfile, DWORD flags) = nullptr;
extern "C" DWORD (*g_GetTempPathW) (DWORD count, LPWSTR path) = nullptr;
extern "C" BOOL (*g_GetDiskFreeSpaceExW) (LPCWSTR root, PULARGE_INTEGER avail, PULARGE_INTEGER total, PULARGE_INTEGER totalfree) = nullptr;
extern "C" INT (*g_WideCharToMultiByte) (UINT page, DWORD flags, LPCWSTR src, INT srclen, LPSTR dst, INT dstlen, LPCSTR defchar, BOOL *used) = nullptr;
extern "C" BOOL (*g_SetConsoleCtrlHandler) (PHANDLER_ROUTINE func, BOOL add) = nullptr;
extern "C" BOOL (*g_GetConsoleScreenBufferInfo) (HANDLE hConsoleOutput, void* csbi) = nullptr;
extern "C" HANDLE (*g_OpenEventW) (DWORD access, BOOL inherit, LPCWSTR name) = nullptr;
extern "C" BOOL (*g_SetEvent) (HANDLE handle) = nullptr;
extern "C" UINT (*g_GetConsoleOutputCP) (VOID) = nullptr;
extern "C" BOOL (*g_WriteFile) (HANDLE hFile, LPCVOID buffer, DWORD bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) = nullptr;
extern "C" UINT (*g_GetACP) (void) = nullptr;
extern "C" BOOL (*g_GetConsoleMode) (HANDLE hcon, LPDWORD mode) = nullptr;
extern "C" BOOL (*g_QueryPerformanceFrequency) (PLARGE_INTEGER frequency) = nullptr;
extern "C" HGLOBAL (*g_LoadResource) (HINSTANCE hModule, HRSRC hRsrc) = nullptr;
extern "C" HRSRC (*g_FindResourceW) (HINSTANCE hModule, LPCWSTR name, LPCWSTR type) = nullptr;
extern "C" DWORD (*g_GetFileType) (HANDLE hFile) = nullptr;
extern "C" BOOL (*g_QueryPerformanceCounter) (PLARGE_INTEGER counter) = nullptr;
extern "C" BOOL (*g_VirtualFree) (LPVOID addr, SIZE_T size, DWORD type) = nullptr;
extern "C" LPVOID (*g_VirtualAlloc) (void *addr, SIZE_T size, DWORD type, DWORD protect) = nullptr;
extern "C" BOOL (*g_UnmapViewOfFile) (LPCVOID addr) = nullptr;
extern "C" HANDLE (*g_CreateFileMappingW) (HANDLE hFile, void* sa, DWORD protect, DWORD size_high, DWORD size_low, LPCWSTR name) = nullptr;
extern "C" LPVOID (*g_MapViewOfFileEx) (HANDLE handle, DWORD access, DWORD offset_high, DWORD offset_low, SIZE_T count, LPVOID addr) = nullptr;
extern "C" VOID (*g_GetStartupInfoW) (LPSTARTUPINFOW info) = nullptr;
extern "C" NTSTATUS (*g_EnterCriticalSection) (void* crit) = nullptr;
extern "C" HANDLE (*g_GetStdHandle) (DWORD std_handle) = nullptr;
extern "C" DWORD (*g_WaitForMultipleObjects) (DWORD count, const HANDLE *handles, BOOL wait_all, DWORD timeout) = nullptr;
extern "C" BOOL (*g_SetEnvironmentVariableW) (LPCWSTR name, LPCWSTR value) = nullptr;
extern "C" BOOL (*g_SetThreadPriority) (HANDLE hthread, INT priority) = nullptr;
extern "C" NTSTATUS (*g_LeaveCriticalSection) (void* crit) = nullptr;
extern "C" BOOL (*g_CreatePipe) (PHANDLE hReadPipe, PHANDLE hWritePipe, void* sa, DWORD size) = nullptr;
extern "C" void (*g_InitializeCriticalSection) (void* crit) = nullptr;
extern "C" BOOL (*g_DuplicateHandle) (HANDLE source_process, HANDLE source, HANDLE dest_process, HANDLE *dest, DWORD access, BOOL inherit, DWORD options) = nullptr;
extern "C" VOID (*g_Sleep) (DWORD timeout) = nullptr;
extern "C" HANDLE (*g_CreateThread) (void* sa, SIZE_T stack, LPTHREAD_START_ROUTINE start, LPVOID param, DWORD flags, LPDWORD id) = nullptr;
extern "C" DWORD (*g_GetCurrentProcessId) (void) = nullptr;
extern "C" LPWSTR (*g_GetEnvironmentStringsW) (void) = nullptr;
extern "C" HANDLE (*g_FindFirstFileW) (LPCWSTR lpFileName, void* lpFindData) = nullptr;
extern "C" BOOLEAN (*g_HeapFree) (HANDLE heap, ULONG flags, PVOID ptr) = nullptr;
extern "C" BOOL (*g_FindNextFileW) (HANDLE handle, void* data) = nullptr;
extern "C" LANGID (*g_GetUserDefaultUILanguage) (void) = nullptr;
extern "C" PVOID (*g_EncodePointer) (PVOID ptr) = nullptr;
extern "C" BOOL (*g_FindClose) (HANDLE handle) = nullptr;
extern "C" PVOID (*g_HeapAlloc) (HANDLE heap, ULONG flags, SIZE_T size) = nullptr;
extern "C" HANDLE (*g_GetProcessHeap) (void) = nullptr;
extern "C" DWORD (*g_ExpandEnvironmentStringsW) (LPCWSTR src, LPWSTR dst, DWORD count) = nullptr;
extern "C" DWORD (*g_GetCurrentThreadId) (void) = nullptr;
extern "C" DWORD (*g_GetVersion) (void) = nullptr;
extern "C" HANDLE (*g_CreateEventW) (void* sa, BOOL manual_reset, BOOL initial_state, LPCWSTR name) = nullptr;
extern "C" BOOL (*g_IsDebuggerPresent) (void) = nullptr;
extern "C" VOID (*g_InitializeSListHead) (PSLIST_HEADER list) = nullptr;
extern "C" VOID (*g_GetSystemTimeAsFileTime) (LPFILETIME time) = nullptr;
extern "C" DWORD (*g_WaitForSingleObjectEx) (HANDLE handle, DWORD timeout, BOOL alertable) = nullptr;
extern "C" BOOL (*g_ResetEvent) (HANDLE handle) = nullptr;
extern "C" BOOL (*g_IsProcessorFeaturePresent) (DWORD feature) = nullptr;
extern "C" BOOL (*g_TerminateProcess) (HANDLE handle, DWORD exit_code) = nullptr;
extern "C" LPTOP_LEVEL_EXCEPTION_FILTER (*g_SetUnhandledExceptionFilter) (LPTOP_LEVEL_EXCEPTION_FILTER filter) = nullptr;
extern "C" LONG (*g_UnhandledExceptionFilter) (PEXCEPTION_POINTERS epointers) = nullptr;
extern "C" PVOID (*g_RtlVirtualUnwind) (ULONG type, ULONG64 base, ULONG64 pc, void* function, void* context, PVOID *data, ULONG64 *frame_ret, void* ctx_ptr) = nullptr;
extern "C" PRUNTIME_FUNCTION (*g_RtlLookupFunctionEntry) (ULONG64 pc, ULONG64 *base, void* table) = nullptr;
extern "C" VOID (*g_RtlCaptureContext) (VOID*) = nullptr;
extern "C" void (*g_OutputDebugStringW) (LPCWSTR str) = nullptr;
extern "C" BOOL (*g_VirtualProtect) (LPVOID addr, SIZE_T size, DWORD new_prot, LPDWORD old_prot) = nullptr;
extern "C" HMODULE (*g_LoadLibraryExA) (LPCSTR libname, HANDLE hfile, DWORD flags) = nullptr;
//extern "C" BOOL (*g_GetVolumeInformationW) (LPCWSTR root, LPWSTR label, DWORD label_len, DWORD *serial, DWORD *filename_len, DWORD *flags, LPWSTR fsname, DWORD fsname_len) = nullptr;
//extern "C" DWORD (*g_GetFinalPathNameByHandleW) (HANDLE file, LPWSTR path, DWORD charcount, DWORD flags) = nullptr;
//extern "C" UINT (*g_GetDriveTypeW) (LPCWSTR root) = nullptr;
//extern "C" DWORD (*g_FormatMessageW) (DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPWSTR lpBuffer, DWORD nSize, void* args) = nullptr;
//extern "C" int (*g_lstrcmpiW) (LPCWSTR str1, LPCWSTR str2) = nullptr;
//extern "C" INT (*g_MultiByteToWideChar) (UINT page, DWORD flags, LPCSTR src, INT srclen, LPWSTR dst, INT dstlen) = nullptr;
//extern "C" DWORD (*g_SizeofResource) (HINSTANCE hModule, HRSRC hRsrc) = nullptr;
//extern "C" BOOL (*g_GetCPInfo) (UINT codepage, LPCPINFO cpinfo) = nullptr;
//extern "C" BOOL (*g_IsValidCodePage) (UINT codepage) = nullptr;
//extern "C" BOOL (*g_GetVersionExW) (void* info) = nullptr;
//extern "C" HANDLE (*g_OpenFileMappingW) (DWORD access, BOOL inherit, LPCWSTR name) = nullptr;
//extern "C" LPVOID (*g_MapViewOfFile) (HANDLE mapping, DWORD access, DWORD offset_high, DWORD offset_low, SIZE_T count) = nullptr;
//extern "C" void (*g_RaiseFailFastException) (DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args) = nullptr;
//extern "C" void (*g_ExitProcess) (DWORD status) = nullptr;
//extern "C" BOOL (*g_ReleaseSemaphore) (HANDLE handle, LONG count, LONG *previous) = nullptr;
//extern "C" HANDLE (*g_CreateSemaphoreW) (void* sa, LONG initial, LONG max, LPCWSTR name) = nullptr;
//extern "C" DWORD (*g_GetFileSize) (HANDLE hFile, LPDWORD filesizehigh) = nullptr;
//extern "C" BOOL (*g_SetProcessWorkingSetSize) (HANDLE hProcess, SIZE_T minset, SIZE_T maxset) = nullptr;
//extern "C" INT (*g_LCMapStringW) (LCID lcid, DWORD flags, LPCWSTR src, INT srclen, LPWSTR dst, INT dstlen) = nullptr;
//extern "C" DWORD (*g_GetLongPathNameW) (LPCWSTR shortpath, LPWSTR longpath, DWORD longlen) = nullptr;
//extern "C" HANDLE (*g_OpenProcess) (DWORD access, BOOL inherit, DWORD id) = nullptr;
//extern "C" void (*g_SetLastError) (DWORD error) = nullptr;
//extern "C" HLOCAL (*g_LocalFree) (HLOCAL handle) = nullptr;
//extern "C" HLOCAL (*g_LocalAlloc) (UINT flags, SIZE_T size) = nullptr;
//extern "C" HMODULE (*g_LoadLibraryA) (LPCSTR libname) = nullptr;
//extern "C" BOOL (*g_LockFileEx) (HANDLE hFile, DWORD flags, DWORD reserved, DWORD count_low, DWORD count_high, LPOVERLAPPED overlapped) = nullptr;
//extern "C" BOOL (*g_UnlockFileEx) (HANDLE hFile, DWORD reserved, DWORD count_low, DWORD count_high, LPOVERLAPPED overlapped) = nullptr;
//extern "C" BOOL (*g_GetFileAttributesExW) (LPCWSTR name, GET_FILEEX_INFO_LEVELS level, LPVOID ptr) = nullptr;
//extern "C" BOOL (*g_GetFileTime) (HANDLE hFile, FILETIME *lpCreationTime, FILETIME *lpLastAccessTime, FILETIME *lpLastWriteTime) = nullptr;
//extern "C" BOOL (*g_SetFileTime) (HANDLE hFile, const FILETIME *ctime, const FILETIME *atime, const FILETIME *mtime) = nullptr;

std::string FileNameFromMappedMem (LPVOID pMem) {
	char buffer[MAX_PATH + 1];
	DWORD len = GetMappedFileNameA (GetCurrentProcess (), pMem, buffer, MAX_PATH);
	if (len > 0) {
		return std::string (buffer, buffer + len);
	}
	return std::string ("<empty>");
}

std::string FileNameFromHandle (HANDLE fileHandle) {
	char buffer[MAX_PATH + 1];
	if (fileHandle > 0) {
		DWORD len = GetFinalPathNameByHandleA (fileHandle, buffer, MAX_PATH, VOLUME_NAME_NT);
		if (len > 0) {
			return std::string (buffer, buffer + len);
		}
	}
	return std::string ("<empty>");
}

static std::string FileNameFromModuleHandle (HMODULE moduleHandle) {
	char buffer[MAX_PATH + 1];
	if (moduleHandle > 0) {
		DWORD len = GetModuleFileNameA (moduleHandle, buffer, MAX_PATH);
		if (len > 0) {
			std::string path (buffer, buffer + len);
			std::string::size_type pos = path.rfind ('\\');
			if (pos != std::string::npos && pos < path.length () - 1) {
				return path.substr (pos + 1);
			}
			return path;
		}
	}
	return std::string ("<empty>");
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	g_logModule = "StubKernel32 (" + FileNameFromModuleHandle (hModule) + ")";
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		std::cout << g_logModule << " DLL_PROCESS_ATTACH called" << std::endl;

		g_moduleHandle = LoadLibraryA ("C:\\WINDOWS\\System32\\kernel32.dll");
		if (g_moduleHandle) {
			g_CreateDirectoryW = (BOOL (*) (LPCWSTR path, void* sa)) GetProcAddress (g_moduleHandle, "CreateDirectoryW");
			if (!g_CreateDirectoryW) {
				return FALSE;
			}

			g_ReadFile = (BOOL (*) (HANDLE hFile, LPVOID buffer, DWORD bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped)) GetProcAddress (g_moduleHandle, "ReadFile");
			if (!g_ReadFile) {
				return FALSE;
			}

			g_GetCommandLineW = (LPWSTR (*) (void)) GetProcAddress (g_moduleHandle, "GetCommandLineW");
			if (!g_GetCommandLineW) {
				return FALSE;
			}

			g_GetCurrentProcess = (HANDLE (*) (void)) GetProcAddress (g_moduleHandle, "GetCurrentProcess");
			if (!g_GetCurrentProcess) {
				return FALSE;
			}

			g_GetModuleFileNameW = (DWORD (*) (HMODULE hModule, LPWSTR lpFileName, DWORD size)) GetProcAddress (g_moduleHandle, "GetModuleFileNameW");
			if (!g_GetModuleFileNameW) {
				return FALSE;
			}

			g_SetErrorMode = (UINT (*) (UINT mode)) GetProcAddress (g_moduleHandle, "SetErrorMode");
			if (!g_SetErrorMode) {
				return FALSE;
			}

			g_SetFilePointer = (DWORD (*) (HANDLE hFile, LONG distance, LONG *highword, DWORD method)) GetProcAddress (g_moduleHandle, "SetFilePointer");
			if (!g_SetFilePointer) {
				return FALSE;
			}

			g_GetEnvironmentVariableW = (DWORD (*) (LPCWSTR name, LPWSTR val, DWORD size)) GetProcAddress (g_moduleHandle, "GetEnvironmentVariableW");
			if (!g_GetEnvironmentVariableW) {
				return FALSE;
			}

			g_CreateMutexW = (HANDLE (*) (void* sa, BOOL owner, LPCWSTR name)) GetProcAddress (g_moduleHandle, "CreateMutexW");
			if (!g_CreateMutexW) {
				return FALSE;
			}

			g_InitializeCriticalSectionEx = (BOOL (*) (void* crit, DWORD spincount, DWORD flags)) GetProcAddress (g_moduleHandle, "InitializeCriticalSectionEx");
			if (!g_InitializeCriticalSectionEx) {
				return FALSE;
			}

			g_WaitForSingleObject = (DWORD (*) (HANDLE handle, DWORD timeout)) GetProcAddress (g_moduleHandle, "WaitForSingleObject");
			if (!g_WaitForSingleObject) {
				return FALSE;
			}

			g_CreateFileW = (HANDLE (*) (LPCWSTR filename, DWORD access, DWORD sharing, void* sa, DWORD creation, DWORD attributes, HANDLE templateParam)) GetProcAddress (g_moduleHandle, "CreateFileW");
			if (!g_CreateFileW) {
				return FALSE;
			}

			g_ReleaseMutex = (BOOL (*) (HANDLE handle)) GetProcAddress (g_moduleHandle, "ReleaseMutex");
			if (!g_ReleaseMutex) {
				return FALSE;
			}

			g_CreateToolhelp32Snapshot = (HANDLE (*) (DWORD flags, DWORD process)) GetProcAddress (g_moduleHandle, "CreateToolhelp32Snapshot");
			if (!g_CreateToolhelp32Snapshot) {
				return FALSE;
			}

			g_GetLastError = (DWORD (*) (void)) GetProcAddress (g_moduleHandle, "GetLastError");
			if (!g_GetLastError) {
				return FALSE;
			}

			g_Process32NextW = (BOOL (*) (HANDLE hSnapshot, void* lppe)) GetProcAddress (g_moduleHandle, "Process32NextW");
			if (!g_Process32NextW) {
				return FALSE;
			}

			g_Process32FirstW = (BOOL (*) (HANDLE hSnapshot, void* lppe)) GetProcAddress (g_moduleHandle, "Process32FirstW");
			if (!g_Process32FirstW) {
				return FALSE;
			}

			g_CloseHandle = (BOOL (*) (HANDLE handle)) GetProcAddress (g_moduleHandle, "CloseHandle");
			if (!g_CloseHandle) {
				return FALSE;
			}

			g_RaiseException = (void (*) (DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args)) GetProcAddress (g_moduleHandle, "RaiseException");
			if (!g_RaiseException) {
				return FALSE;
			}

			g_GetSystemInfo = (VOID (*) (LPSYSTEM_INFO si)) GetProcAddress (g_moduleHandle, "GetSystemInfo");
			if (!g_GetSystemInfo) {
				return FALSE;
			}

			g_LoadLibraryW = (HMODULE (*) (LPCWSTR libnameW)) GetProcAddress (g_moduleHandle, "LoadLibraryW");
			if (!g_LoadLibraryW) {
				return FALSE;
			}

			g_SwitchToThread = (BOOL (*) (void)) GetProcAddress (g_moduleHandle, "SwitchToThread");
			if (!g_SwitchToThread) {
				return FALSE;
			}

			g_DecodePointer = (void* (*) (void * ptr)) GetProcAddress (g_moduleHandle, "DecodePointer");
			if (!g_DecodePointer) {
				return FALSE;
			}

			g_GetProcAddress = (FARPROC (*) (HMODULE hModule, LPCSTR function)) GetProcAddress (g_moduleHandle, "GetProcAddress");
			if (!g_GetProcAddress) {
				return FALSE;
			}

			g_DeleteCriticalSection = (void (*) (void* crit)) GetProcAddress (g_moduleHandle, "DeleteCriticalSection");
			if (!g_DeleteCriticalSection) {
				return FALSE;
			}

			g_CreateProcessW = (BOOL (*) (LPCWSTR app_name, LPWSTR cmd_line, void* process_attr, void* thread_attr, BOOL inherit, DWORD flags, LPVOID env, LPCWSTR cur_dir, LPSTARTUPINFOW startup_info, LPPROCESS_INFORMATION info)) GetProcAddress (g_moduleHandle, "CreateProcessW");
			if (!g_CreateProcessW) {
				return FALSE;
			}

			g_GetModuleHandleW = (HMODULE (*) (LPCWSTR module)) GetProcAddress (g_moduleHandle, "GetModuleHandleW");
			if (!g_GetModuleHandleW) {
				return FALSE;
			}

			g_FreeLibrary = (BOOL (*) (HINSTANCE hLibModule)) GetProcAddress (g_moduleHandle, "FreeLibrary");
			if (!g_FreeLibrary) {
				return FALSE;
			}

			g_OpenMutexW = (HANDLE (*) (DWORD access, BOOL inherit, LPCWSTR name)) GetProcAddress (g_moduleHandle, "OpenMutexW");
			if (!g_OpenMutexW) {
				return FALSE;
			}

			g_VirtualQuery = (SIZE_T (*) (LPCVOID addr, PMEMORY_BASIC_INFORMATION info, SIZE_T len)) GetProcAddress (g_moduleHandle, "VirtualQuery");
			if (!g_VirtualQuery) {
				return FALSE;
			}

			g_LoadLibraryExW = (HMODULE (*) (LPCWSTR libnameW, HANDLE hfile, DWORD flags)) GetProcAddress (g_moduleHandle, "LoadLibraryExW");
			if (!g_LoadLibraryExW) {
				return FALSE;
			}

			g_GetTempPathW = (DWORD (*) (DWORD count, LPWSTR path)) GetProcAddress (g_moduleHandle, "GetTempPathW");
			if (!g_GetTempPathW) {
				return FALSE;
			}

			g_GetDiskFreeSpaceExW = (BOOL (*) (LPCWSTR root, PULARGE_INTEGER avail, PULARGE_INTEGER total, PULARGE_INTEGER totalfree)) GetProcAddress (g_moduleHandle, "GetDiskFreeSpaceExW");
			if (!g_GetDiskFreeSpaceExW) {
				return FALSE;
			}

			g_WideCharToMultiByte = (INT (*) (UINT page, DWORD flags, LPCWSTR src, INT srclen, LPSTR dst, INT dstlen, LPCSTR defchar, BOOL *used)) GetProcAddress (g_moduleHandle, "WideCharToMultiByte");
			if (!g_WideCharToMultiByte) {
				return FALSE;
			}

			g_SetConsoleCtrlHandler = (BOOL (*) (PHANDLER_ROUTINE func, BOOL add)) GetProcAddress (g_moduleHandle, "SetConsoleCtrlHandler");
			if (!g_SetConsoleCtrlHandler) {
				return FALSE;
			}

			g_GetConsoleScreenBufferInfo = (BOOL (*) (HANDLE hConsoleOutput, void* csbi)) GetProcAddress (g_moduleHandle, "GetConsoleScreenBufferInfo");
			if (!g_GetConsoleScreenBufferInfo) {
				return FALSE;
			}

			g_OpenEventW = (HANDLE (*) (DWORD access, BOOL inherit, LPCWSTR name)) GetProcAddress (g_moduleHandle, "OpenEventW");
			if (!g_OpenEventW) {
				return FALSE;
			}

			g_SetEvent = (BOOL (*) (HANDLE handle)) GetProcAddress (g_moduleHandle, "SetEvent");
			if (!g_SetEvent) {
				return FALSE;
			}

			g_GetConsoleOutputCP = (UINT (*) (VOID)) GetProcAddress (g_moduleHandle, "GetConsoleOutputCP");
			if (!g_GetConsoleOutputCP) {
				return FALSE;
			}

			g_WriteFile = (BOOL (*) (HANDLE hFile, LPCVOID buffer, DWORD bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped)) GetProcAddress (g_moduleHandle, "WriteFile");
			if (!g_WriteFile) {
				return FALSE;
			}

			g_GetACP = (UINT (*) (void)) GetProcAddress (g_moduleHandle, "GetACP");
			if (!g_GetACP) {
				return FALSE;
			}

			g_GetConsoleMode = (BOOL (*) (HANDLE hcon, LPDWORD mode)) GetProcAddress (g_moduleHandle, "GetConsoleMode");
			if (!g_GetConsoleMode) {
				return FALSE;
			}

			g_QueryPerformanceFrequency = (BOOL (*) (PLARGE_INTEGER frequency)) GetProcAddress (g_moduleHandle, "QueryPerformanceFrequency");
			if (!g_QueryPerformanceFrequency) {
				return FALSE;
			}

			g_LoadResource = (HGLOBAL (*) (HINSTANCE hModule, HRSRC hRsrc)) GetProcAddress (g_moduleHandle, "LoadResource");
			if (!g_LoadResource) {
				return FALSE;
			}

			g_FindResourceW = (HRSRC (*) (HINSTANCE hModule, LPCWSTR name, LPCWSTR type)) GetProcAddress (g_moduleHandle, "FindResourceW");
			if (!g_FindResourceW) {
				return FALSE;
			}

			g_GetFileType = (DWORD (*) (HANDLE hFile)) GetProcAddress (g_moduleHandle, "GetFileType");
			if (!g_GetFileType) {
				return FALSE;
			}

			g_QueryPerformanceCounter = (BOOL (*) (PLARGE_INTEGER counter)) GetProcAddress (g_moduleHandle, "QueryPerformanceCounter");
			if (!g_QueryPerformanceCounter) {
				return FALSE;
			}

			g_VirtualFree = (BOOL (*) (LPVOID addr, SIZE_T size, DWORD type)) GetProcAddress (g_moduleHandle, "VirtualFree");
			if (!g_VirtualFree) {
				return FALSE;
			}

			g_VirtualAlloc = (LPVOID (*) (void *addr, SIZE_T size, DWORD type, DWORD protect)) GetProcAddress (g_moduleHandle, "VirtualAlloc");
			if (!g_VirtualAlloc) {
				return FALSE;
			}

			g_UnmapViewOfFile = (BOOL (*) (LPCVOID addr)) GetProcAddress (g_moduleHandle, "UnmapViewOfFile");
			if (!g_UnmapViewOfFile) {
				return FALSE;
			}

			g_CreateFileMappingW = (HANDLE (*) (HANDLE hFile, void* sa, DWORD protect, DWORD size_high, DWORD size_low, LPCWSTR name)) GetProcAddress (g_moduleHandle, "CreateFileMappingW");
			if (!g_CreateFileMappingW) {
				return FALSE;
			}

			g_MapViewOfFileEx = (LPVOID (*) (HANDLE handle, DWORD access, DWORD offset_high, DWORD offset_low, SIZE_T count, LPVOID addr)) GetProcAddress (g_moduleHandle, "MapViewOfFileEx");
			if (!g_MapViewOfFileEx) {
				return FALSE;
			}

			g_GetStartupInfoW = (VOID (*) (LPSTARTUPINFOW info)) GetProcAddress (g_moduleHandle, "GetStartupInfoW");
			if (!g_GetStartupInfoW) {
				return FALSE;
			}

			g_EnterCriticalSection = (NTSTATUS (*) (void* crit)) GetProcAddress (g_moduleHandle, "EnterCriticalSection");
			if (!g_EnterCriticalSection) {
				return FALSE;
			}

			g_GetStdHandle = (HANDLE (*) (DWORD std_handle)) GetProcAddress (g_moduleHandle, "GetStdHandle");
			if (!g_GetStdHandle) {
				return FALSE;
			}

			g_WaitForMultipleObjects = (DWORD (*) (DWORD count, const HANDLE *handles, BOOL wait_all, DWORD timeout)) GetProcAddress (g_moduleHandle, "WaitForMultipleObjects");
			if (!g_WaitForMultipleObjects) {
				return FALSE;
			}

			g_SetEnvironmentVariableW = (BOOL (*) (LPCWSTR name, LPCWSTR value)) GetProcAddress (g_moduleHandle, "SetEnvironmentVariableW");
			if (!g_SetEnvironmentVariableW) {
				return FALSE;
			}

			g_SetThreadPriority = (BOOL (*) (HANDLE hthread, INT priority)) GetProcAddress (g_moduleHandle, "SetThreadPriority");
			if (!g_SetThreadPriority) {
				return FALSE;
			}

			g_LeaveCriticalSection = (NTSTATUS (*) (void* crit)) GetProcAddress (g_moduleHandle, "LeaveCriticalSection");
			if (!g_LeaveCriticalSection) {
				return FALSE;
			}

			g_CreatePipe = (BOOL (*) (PHANDLE hReadPipe, PHANDLE hWritePipe, void* sa, DWORD size)) GetProcAddress (g_moduleHandle, "CreatePipe");
			if (!g_CreatePipe) {
				return FALSE;
			}

			g_InitializeCriticalSection = (void (*) (void* crit)) GetProcAddress (g_moduleHandle, "InitializeCriticalSection");
			if (!g_InitializeCriticalSection) {
				return FALSE;
			}

			g_DuplicateHandle = (BOOL (*) (HANDLE source_process, HANDLE source, HANDLE dest_process, HANDLE *dest, DWORD access, BOOL inherit, DWORD options)) GetProcAddress (g_moduleHandle, "DuplicateHandle");
			if (!g_DuplicateHandle) {
				return FALSE;
			}

			g_Sleep = (VOID (*) (DWORD timeout)) GetProcAddress (g_moduleHandle, "Sleep");
			if (!g_Sleep) {
				return FALSE;
			}

			g_CreateThread = (HANDLE (*) (void* sa, SIZE_T stack, LPTHREAD_START_ROUTINE start, LPVOID param, DWORD flags, LPDWORD id)) GetProcAddress (g_moduleHandle, "CreateThread");
			if (!g_CreateThread) {
				return FALSE;
			}

			g_GetCurrentProcessId = (DWORD (*) (void)) GetProcAddress (g_moduleHandle, "GetCurrentProcessId");
			if (!g_GetCurrentProcessId) {
				return FALSE;
			}

			g_GetEnvironmentStringsW = (LPWSTR (*) (void)) GetProcAddress (g_moduleHandle, "GetEnvironmentStringsW");
			if (!g_GetEnvironmentStringsW) {
				return FALSE;
			}

			g_FindFirstFileW = (HANDLE (*) (LPCWSTR lpFileName, void* lpFindData)) GetProcAddress (g_moduleHandle, "FindFirstFileW");
			if (!g_FindFirstFileW) {
				return FALSE;
			}

			g_HeapFree = (BOOLEAN (*) (HANDLE heap, ULONG flags, PVOID ptr)) GetProcAddress (g_moduleHandle, "HeapFree");
			if (!g_HeapFree) {
				return FALSE;
			}

			g_FindNextFileW = (BOOL (*) (HANDLE handle, void* data)) GetProcAddress (g_moduleHandle, "FindNextFileW");
			if (!g_FindNextFileW) {
				return FALSE;
			}

			g_GetUserDefaultUILanguage = (LANGID (*) (void)) GetProcAddress (g_moduleHandle, "GetUserDefaultUILanguage");
			if (!g_GetUserDefaultUILanguage) {
				return FALSE;
			}

			g_EncodePointer = (PVOID (*) (PVOID ptr)) GetProcAddress (g_moduleHandle, "EncodePointer");
			if (!g_EncodePointer) {
				return FALSE;
			}

			g_FindClose = (BOOL (*) (HANDLE handle)) GetProcAddress (g_moduleHandle, "FindClose");
			if (!g_FindClose) {
				return FALSE;
			}

			g_HeapAlloc = (PVOID (*) (HANDLE heap, ULONG flags, SIZE_T size)) GetProcAddress (g_moduleHandle, "HeapAlloc");
			if (!g_HeapAlloc) {
				return FALSE;
			}

			g_GetProcessHeap = (HANDLE (*) (void)) GetProcAddress (g_moduleHandle, "GetProcessHeap");
			if (!g_GetProcessHeap) {
				return FALSE;
			}

			g_ExpandEnvironmentStringsW = (DWORD (*) (LPCWSTR src, LPWSTR dst, DWORD count)) GetProcAddress (g_moduleHandle, "ExpandEnvironmentStringsW");
			if (!g_ExpandEnvironmentStringsW) {
				return FALSE;
			}

			g_GetCurrentThreadId = (DWORD (*) (void)) GetProcAddress (g_moduleHandle, "GetCurrentThreadId");
			if (!g_GetCurrentThreadId) {
				return FALSE;
			}

			g_GetVersion = (DWORD (*) (void)) GetProcAddress (g_moduleHandle, "GetVersion");
			if (!g_GetVersion) {
				return FALSE;
			}

			g_CreateEventW = (HANDLE (*) (void* sa, BOOL manual_reset, BOOL initial_state, LPCWSTR name)) GetProcAddress (g_moduleHandle, "CreateEventW");
			if (!g_CreateEventW) {
				return FALSE;
			}

			g_IsDebuggerPresent = (BOOL (*) (void)) GetProcAddress (g_moduleHandle, "IsDebuggerPresent");
			if (!g_IsDebuggerPresent) {
				return FALSE;
			}

			g_InitializeSListHead = (VOID (*) (PSLIST_HEADER list)) GetProcAddress (g_moduleHandle, "InitializeSListHead");
			if (!g_InitializeSListHead) {
				return FALSE;
			}

			g_GetSystemTimeAsFileTime = (VOID (*) (LPFILETIME time)) GetProcAddress (g_moduleHandle, "GetSystemTimeAsFileTime");
			if (!g_GetSystemTimeAsFileTime) {
				return FALSE;
			}

			g_WaitForSingleObjectEx = (DWORD (*) (HANDLE handle, DWORD timeout, BOOL alertable)) GetProcAddress (g_moduleHandle, "WaitForSingleObjectEx");
			if (!g_WaitForSingleObjectEx) {
				return FALSE;
			}

			g_ResetEvent = (BOOL (*) (HANDLE handle)) GetProcAddress (g_moduleHandle, "ResetEvent");
			if (!g_ResetEvent) {
				return FALSE;
			}

			g_IsProcessorFeaturePresent = (BOOL (*) (DWORD feature)) GetProcAddress (g_moduleHandle, "IsProcessorFeaturePresent");
			if (!g_IsProcessorFeaturePresent) {
				return FALSE;
			}

			g_TerminateProcess = (BOOL (*) (HANDLE handle, DWORD exit_code)) GetProcAddress (g_moduleHandle, "TerminateProcess");
			if (!g_TerminateProcess) {
				return FALSE;
			}

			g_SetUnhandledExceptionFilter = (LPTOP_LEVEL_EXCEPTION_FILTER (*) (LPTOP_LEVEL_EXCEPTION_FILTER filter)) GetProcAddress (g_moduleHandle, "SetUnhandledExceptionFilter");
			if (!g_SetUnhandledExceptionFilter) {
				return FALSE;
			}

			g_UnhandledExceptionFilter = (LONG (*) (PEXCEPTION_POINTERS epointers)) GetProcAddress (g_moduleHandle, "UnhandledExceptionFilter");
			if (!g_UnhandledExceptionFilter) {
				return FALSE;
			}

			g_RtlVirtualUnwind = (PVOID (*) (ULONG type, ULONG64 base, ULONG64 pc, void* function, void* context, PVOID *data, ULONG64 *frame_ret, void* ctx_ptr)) GetProcAddress (g_moduleHandle, "RtlVirtualUnwind");
			if (!g_RtlVirtualUnwind) {
				return FALSE;
			}

			g_RtlLookupFunctionEntry = (PRUNTIME_FUNCTION (*) (ULONG64 pc, ULONG64 *base, void* table)) GetProcAddress (g_moduleHandle, "RtlLookupFunctionEntry");
			if (!g_RtlLookupFunctionEntry) {
				return FALSE;
			}

			g_RtlCaptureContext = (VOID (*) (VOID*)) GetProcAddress (g_moduleHandle, "RtlCaptureContext");
			if (!g_RtlCaptureContext) {
				return FALSE;
			}

			g_OutputDebugStringW = (void (*) (LPCWSTR str)) GetProcAddress (g_moduleHandle, "OutputDebugStringW");
			if (!g_OutputDebugStringW) {
				return FALSE;
			}

			g_VirtualProtect = (BOOL (*) (LPVOID addr, SIZE_T size, DWORD new_prot, LPDWORD old_prot)) GetProcAddress (g_moduleHandle, "VirtualProtect");
			if (!g_VirtualProtect) {
				return FALSE;
			}

			g_LoadLibraryExA = (HMODULE (*) (LPCSTR libname, HANDLE hfile, DWORD flags)) GetProcAddress (g_moduleHandle, "LoadLibraryExA");
			if (!g_LoadLibraryExA) {
				return FALSE;
			}

			//g_GetVolumeInformationW = (BOOL (*) (LPCWSTR root, LPWSTR label, DWORD label_len, DWORD *serial, DWORD *filename_len, DWORD *flags, LPWSTR fsname, DWORD fsname_len)) GetProcAddress (g_moduleHandle, "GetVolumeInformationW");
			//if (!g_GetVolumeInformationW) {
			//	return FALSE;
			//}

			//g_GetFinalPathNameByHandleW = (DWORD (*) (HANDLE file, LPWSTR path, DWORD charcount, DWORD flags)) GetProcAddress (g_moduleHandle, "GetFinalPathNameByHandleW");
			//if (!g_GetFinalPathNameByHandleW) {
			//	return FALSE;
			//}

			//g_GetDriveTypeW = (UINT (*) (LPCWSTR root)) GetProcAddress (g_moduleHandle, "GetDriveTypeW");
			//if (!g_GetDriveTypeW) {
			//	return FALSE;
			//}

			//g_FormatMessageW = (DWORD (*) (DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPWSTR lpBuffer, DWORD nSize, void* args)) GetProcAddress (g_moduleHandle, "FormatMessageW");
			//if (!g_FormatMessageW) {
			//	return FALSE;
			//}

			//g_lstrcmpiW = (int (*) (LPCWSTR str1, LPCWSTR str2)) GetProcAddress (g_moduleHandle, "lstrcmpiW");
			//if (!g_lstrcmpiW) {
			//	return FALSE;
			//}

			//g_MultiByteToWideChar = (INT (*) (UINT page, DWORD flags, LPCSTR src, INT srclen, LPWSTR dst, INT dstlen)) GetProcAddress (g_moduleHandle, "MultiByteToWideChar");
			//if (!g_MultiByteToWideChar) {
			//	return FALSE;
			//}

			//g_SizeofResource = (DWORD (*) (HINSTANCE hModule, HRSRC hRsrc)) GetProcAddress (g_moduleHandle, "SizeofResource");
			//if (!g_SizeofResource) {
			//	return FALSE;
			//}

			//g_GetCPInfo = (BOOL (*) (UINT codepage, LPCPINFO cpinfo)) GetProcAddress (g_moduleHandle, "GetCPInfo");
			//if (!g_GetCPInfo) {
			//	return FALSE;
			//}

			//g_IsValidCodePage = (BOOL (*) (UINT codepage)) GetProcAddress (g_moduleHandle, "IsValidCodePage");
			//if (!g_IsValidCodePage) {
			//	return FALSE;
			//}

			//g_GetVersionExW = (BOOL (*) (void* info)) GetProcAddress (g_moduleHandle, "GetVersionExW");
			//if (!g_GetVersionExW) {
			//	return FALSE;
			//}

			//g_OpenFileMappingW = (HANDLE (*) (DWORD access, BOOL inherit, LPCWSTR name)) GetProcAddress (g_moduleHandle, "OpenFileMappingW");
			//if (!g_OpenFileMappingW) {
			//	return FALSE;
			//}

			//g_MapViewOfFile = (LPVOID (*) (HANDLE mapping, DWORD access, DWORD offset_high, DWORD offset_low, SIZE_T count)) GetProcAddress (g_moduleHandle, "MapViewOfFile");
			//if (!g_MapViewOfFile) {
			//	return FALSE;
			//}

			//g_RaiseFailFastException = (void (*) (DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args)) GetProcAddress (g_moduleHandle, "RaiseFailFastException");
			//if (!g_RaiseFailFastException) {
			//	return FALSE;
			//}

			//g_ExitProcess = (void (*) (DWORD status)) GetProcAddress (g_moduleHandle, "ExitProcess");
			//if (!g_ExitProcess) {
			//	return FALSE;
			//}

			//g_ReleaseSemaphore = (BOOL (*) (HANDLE handle, LONG count, LONG *previous)) GetProcAddress (g_moduleHandle, "ReleaseSemaphore");
			//if (!g_ReleaseSemaphore) {
			//	return FALSE;
			//}

			//g_CreateSemaphoreW = (HANDLE (*) (void* sa, LONG initial, LONG max, LPCWSTR name)) GetProcAddress (g_moduleHandle, "CreateSemaphoreW");
			//if (!g_CreateSemaphoreW) {
			//	return FALSE;
			//}

			//g_GetFileSize = (DWORD (*) (HANDLE hFile, LPDWORD filesizehigh)) GetProcAddress (g_moduleHandle, "GetFileSize");
			//if (!g_GetFileSize) {
			//	return FALSE;
			//}

			//g_SetProcessWorkingSetSize = (BOOL (*) (HANDLE hProcess, SIZE_T minset, SIZE_T maxset)) GetProcAddress (g_moduleHandle, "SetProcessWorkingSetSize");
			//if (!g_SetProcessWorkingSetSize) {
			//	return FALSE;
			//}

			//g_LCMapStringW = (INT (*) (LCID lcid, DWORD flags, LPCWSTR src, INT srclen, LPWSTR dst, INT dstlen)) GetProcAddress (g_moduleHandle, "LCMapStringW");
			//if (!g_LCMapStringW) {
			//	return FALSE;
			//}

			//g_GetLongPathNameW = (DWORD (*) (LPCWSTR shortpath, LPWSTR longpath, DWORD longlen)) GetProcAddress (g_moduleHandle, "GetLongPathNameW");
			//if (!g_GetLongPathNameW) {
			//	return FALSE;
			//}

			//g_OpenProcess = (HANDLE (*) (DWORD access, BOOL inherit, DWORD id)) GetProcAddress (g_moduleHandle, "OpenProcess");
			//if (!g_OpenProcess) {
			//	return FALSE;
			//}

			//g_SetLastError = (void (*) (DWORD error)) GetProcAddress (g_moduleHandle, "SetLastError");
			//if (!g_SetLastError) {
			//	return FALSE;
			//}

			//g_LocalFree = (HLOCAL (*) (HLOCAL handle)) GetProcAddress (g_moduleHandle, "LocalFree");
			//if (!g_LocalFree) {
			//	return FALSE;
			//}

			//g_LocalAlloc = (HLOCAL (*) (UINT flags, SIZE_T size)) GetProcAddress (g_moduleHandle, "LocalAlloc");
			//if (!g_LocalAlloc) {
			//	return FALSE;
			//}

			//g_LoadLibraryA = (HMODULE (*) (LPCSTR libname)) GetProcAddress (g_moduleHandle, "LoadLibraryA");
			//if (!g_LoadLibraryA) {
			//	return FALSE;
			//}

			//g_LockFileEx = (BOOL (*) (HANDLE hFile, DWORD flags, DWORD reserved, DWORD count_low, DWORD count_high, LPOVERLAPPED overlapped)) GetProcAddress (g_moduleHandle, "LockFileEx");
			//if (!g_LockFileEx) {
			//	return FALSE;
			//}

			//g_UnlockFileEx = (BOOL (*) (HANDLE hFile, DWORD reserved, DWORD count_low, DWORD count_high, LPOVERLAPPED overlapped)) GetProcAddress (g_moduleHandle, "UnlockFileEx");
			//if (!g_UnlockFileEx) {
			//	return FALSE;
			//}

			//g_GetFileAttributesExW = (BOOL (*) (LPCWSTR name, GET_FILEEX_INFO_LEVELS level, LPVOID ptr)) GetProcAddress (g_moduleHandle, "GetFileAttributesExW");
			//if (!g_GetFileAttributesExW) {
			//	return FALSE;
			//}

			//g_GetFileTime = (BOOL (*) (HANDLE hFile, FILETIME *lpCreationTime, FILETIME *lpLastAccessTime, FILETIME *lpLastWriteTime)) GetProcAddress (g_moduleHandle, "GetFileTime");
			//if (!g_GetFileTime) {
			//	return FALSE;
			//}

			//g_SetFileTime = (BOOL (*) (HANDLE hFile, const FILETIME *ctime, const FILETIME *atime, const FILETIME *mtime)) GetProcAddress (g_moduleHandle, "SetFileTime");
			//if (!g_SetFileTime) {
			//	return FALSE;
			//}
		}
		break;
	case DLL_THREAD_ATTACH:
		std::cout << g_logModule << " DLL_THREAD_ATTACH called" << std::endl;
		break;
	case DLL_THREAD_DETACH:
		std::cout << g_logModule << " DLL_THREAD_DETACH called" << std::endl;
		break;
	case DLL_PROCESS_DETACH:
		std::cout << g_logModule << " DLL_PROCESS_DETACH called" << std::endl;

		if (g_moduleHandle) {
			FreeLibrary (g_moduleHandle);
			g_moduleHandle = nullptr;
		}
		break;
    }
    return TRUE;
}

