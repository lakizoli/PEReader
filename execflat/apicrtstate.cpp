#include "stdafx.h"
#include "apicrtstate.hpp"

IMPLEMENT_IMPORT_STATE_ITEM (ApiCrtState, "ApiCrt");

ApiCrtState::ApiCrtState () :
	CRT___argc (0),
	CRT_argc_expand (0),
	CRT_wargc_expand (0),
	CRT__commode (0),
	CRT__fmode (0),
	CRT__osver (0),
	CRT__osplatform (0),
	CRT__winmajor (0),
	CRT__winminor (0),
	CRT__winver (0),
	CRT___setlc_active (0),
	CRT___unguarded_readlc_active (0),
	CRT__HUGE (0),
	CRT___argv (nullptr),
	CRT_argv_expand (nullptr),
	CRT___wargv (nullptr),
	CRT_wargv_expand (nullptr),
	CRT__acmdln (nullptr),
	CRT__wcmdln (nullptr),
	CRT__environ (nullptr),
	CRT__wenviron (nullptr),
	CRT___initenv (nullptr),
	CRT___winitenv (nullptr),
	CRT_app_type (0),
	CRT__pgmptr (nullptr),
	CRT__wpgmptr (nullptr)
{
}
