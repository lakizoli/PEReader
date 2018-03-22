#pragma once

#include "importstate.hpp"

//statics ignored, because we execute only one program at once!
struct ApiCrtState : public ImportStateItem {
	DECLARE_IMPORT_STATE_ITEM (ApiCrtState);
	
	int32_t CRT___argc;
	/*static*/ int32_t CRT_argc_expand;
	/*static*/ int32_t CRT_wargc_expand;
	uint32_t CRT__commode;
	uint32_t CRT__fmode;
	uint32_t CRT__osver;
	uint32_t CRT__osplatform;
	uint32_t CRT__winmajor;
	uint32_t CRT__winminor;
	uint32_t CRT__winver;
	uint32_t CRT___setlc_active;
	uint32_t CRT___unguarded_readlc_active;
	double CRT__HUGE;
	char **CRT___argv;
	/*static*/ char **CRT_argv_expand;
	wchar_t **CRT___wargv;
	/*static*/ wchar_t **CRT_wargv_expand;
	char *CRT__acmdln;
	wchar_t *CRT__wcmdln;
	char **CRT__environ;
	wchar_t **CRT__wenviron;
	char **CRT___initenv;
	wchar_t **CRT___winitenv;
	int32_t CRT_app_type;
	char* CRT__pgmptr;
	wchar_t* CRT__wpgmptr;

public:
	ApiCrtState ();
};
