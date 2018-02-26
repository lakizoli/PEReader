#pragma once

#define WIN32

//Logging definitions
//#if BX_NO_LOGGING

#include <cassert>

#define BX_INFO(x)
#define BX_DEBUG(x)
#define BX_ERROR(x)
#define BX_PANIC(x) assert("panic!") //(LOG_THIS panic) x
#define BX_FATAL(x) assert("fatal1!") //(LOG_THIS fatal1) x

#define BX_ASSERT(x)

//#else
//
//#define BX_INFO(x)  (LOG_THIS info) x
//#define BX_DEBUG(x) (LOG_THIS ldebug) x
//#define BX_ERROR(x) (LOG_THIS error) x
//#define BX_PANIC(x) (LOG_THIS panic) x
//#define BX_FATAL(x) (LOG_THIS fatal1) x
//
//#if BX_ASSERT_ENABLE
//#define BX_ASSERT(x) do {if (!(x)) BX_PANIC(("failed assertion \"%s\" at %s:%d\n", #x, __FILE__, __LINE__));} while (0)
//#else
//#define BX_ASSERT(x)
//#endif
//
//#endif 

// Log level defines
typedef enum {
	LOGLEV_DEBUG = 0,
	LOGLEV_INFO,
	LOGLEV_ERROR,
	LOGLEV_PANIC,
	N_LOGLEV
} bx_log_levels;
