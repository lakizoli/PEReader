#pragma once

#define DECLSPEC_HOTPATCH

typedef void *HANDLE;
typedef HANDLE *PHANDLE;

#define DECLARE_HANDLE(name) typedef void* name; //struct name##__{int unused;}; typedef struct name##__ *name

DECLARE_HANDLE (HKEY);
typedef HKEY *PHKEY;
DECLARE_HANDLE (HMETAFILE);
DECLARE_HANDLE (HINSTANCE);
typedef HINSTANCE HMODULE;      /* HMODULEs can be used in place of HINSTANCEs */
DECLARE_HANDLE (HRGN);
DECLARE_HANDLE (HRSRC);
DECLARE_HANDLE (HSPRITE);
DECLARE_HANDLE (HLSURF);
DECLARE_HANDLE (HSTR);
DECLARE_HANDLE (HTASK);
DECLARE_HANDLE (HWINSTA);
DECLARE_HANDLE (HKL);

typedef HANDLE              HGLOBAL;
typedef HANDLE              HLOCAL;

#ifndef UNALIGNED
#if defined(_MSC_VER) && (defined(_M_MRX000) || defined(_M_ALPHA) || defined(_M_PPC) || defined(_M_IA64) || defined(_M_AMD64))
# define UNALIGNED __unaligned
# ifdef _WIN64
#  define UNALIGNED64 __unaligned
# else
#  define UNALIGNED64
# endif
#else
# define UNALIGNED
# define UNALIGNED64
#endif
#endif 

#if defined(_WIN64)
	typedef __int64 INT_PTR, *PINT_PTR;
	typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

	typedef __int64 LONG_PTR, *PLONG_PTR;
	typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

	#define __int3264   __int64
#else
	typedef _W64 int INT_PTR, *PINT_PTR;
	typedef _W64 unsigned int UINT_PTR, *PUINT_PTR;

	typedef _W64 long LONG_PTR, *PLONG_PTR;
	typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;

	#define __int3264   __int32
#endif

#define far
#define near

#if defined(DOSWIN32) || defined(_MAC)
#define cdecl _cdecl
#ifndef CDECL
#define CDECL _cdecl
#endif
#else
#define cdecl
#ifndef CDECL
#define CDECL
#endif
#endif

#undef FAR
#undef  NEAR
#define FAR                 far
#define NEAR                near
#define CONST               const

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef BYTE  BOOLEAN;
typedef BOOLEAN *PBOOLEAN;

#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef int INT;

typedef LONG NTSTATUS;
typedef NTSTATUS *PNTSTATUS;

typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;

typedef void *PVOID;
typedef USHORT LANGID;

typedef unsigned __int64 ULONG64, *PULONG64;
typedef unsigned __int64 DWORD64, *PDWORD64;

typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character

typedef WCHAR *PWCHAR, *LPWCH, *PWCH;
typedef CONST WCHAR *LPCWCH, *PCWCH;

typedef WCHAR *NWPSTR, *LPWSTR, *PWSTR;
typedef PWSTR *PZPWSTR;
typedef CONST PWSTR *PCZPWSTR;
typedef WCHAR UNALIGNED *LPUWSTR, *PUWSTR;
typedef CONST WCHAR *LPCWSTR, *PCWSTR;
typedef PCWSTR *PZPCWSTR;
typedef CONST PCWSTR *PCZPCWSTR;
typedef CONST WCHAR UNALIGNED *LPCUWSTR, *PCUWSTR;

typedef WCHAR *PZZWSTR;
typedef CONST WCHAR *PCZZWSTR;
typedef WCHAR UNALIGNED *PUZZWSTR;
typedef CONST WCHAR UNALIGNED *PCUZZWSTR;

typedef  WCHAR *PNZWCH;
typedef  CONST WCHAR *PCNZWCH;
typedef  WCHAR UNALIGNED *PUNZWCH;
typedef  CONST WCHAR UNALIGNED *PCUNZWCH;

typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCCH, *PCCH;

typedef CHAR *NPSTR, *LPSTR, *PSTR;
typedef PSTR *PZPSTR;
typedef CONST PSTR *PCZPSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;
typedef PCSTR *PZPCSTR;
typedef CONST PCSTR *PCZPCSTR;

typedef CHAR *PZZSTR;
typedef CONST CHAR *PCZZSTR;

typedef  CHAR *PNZCH;
typedef  CONST CHAR *PCNZCH;

#define MAX_PATH          260

typedef void* LPOVERLAPPED;
typedef void* LPSYSTEM_INFO;
typedef void* LPSTARTUPINFOW;
typedef void* LPPROCESS_INFORMATION;
typedef void* PMEMORY_BASIC_INFORMATION;
typedef void* PULARGE_INTEGER;
typedef void* PLARGE_INTEGER;
typedef void* PHANDLER_ROUTINE;
typedef void* LPTHREAD_START_ROUTINE;
typedef void* FARPROC;
typedef void* PSLIST_HEADER;
typedef void* LPFILETIME;
typedef void* LPTOP_LEVEL_EXCEPTION_FILTER;
typedef void* PEXCEPTION_POINTERS;
typedef void* PRUNTIME_FUNCTION;
typedef void* LPCPINFO;

typedef ULONG_PTR SIZE_T, *PSIZE_T;
//typedef LONG_PTR SSIZE_T, *PSSIZE_T;

typedef ULONG64 FILETIME;

#define FALSE 0
#define TRUE 1

typedef enum _GET_FILEEX_INFO_LEVELS {
	GetFileExInfoStandard,
	GetFileExMaxInfoLevel
} GET_FILEEX_INFO_LEVELS;

typedef char CCHAR;
typedef DWORD LCID;
typedef PDWORD PLCID;
typedef WORD   LANGID;
