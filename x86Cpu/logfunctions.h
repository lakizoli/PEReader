#pragma once

#include <cstdarg>

typedef class logfunctions {
public:
	logfunctions (void) {}
	logfunctions (class iofunctions *) {}
	virtual ~logfunctions (void) {}

	void info (const char *fmt, ...) {} //BX_CPP_AttrPrintf (2, 3);
	void error (const char *fmt, ...) {} //BX_CPP_AttrPrintf (2, 3);
	void panic (const char *fmt, ...) {} //BX_CPP_AttrPrintf (2, 3);
	void ldebug (const char *fmt, ...) {} //BX_CPP_AttrPrintf (2, 3);
	void fatal1 (const char *fmt, ...) {}//BX_CPP_AttrPrintf (2, 3);
	void fatal (int level, const char *prefix, const char *fmt, va_list ap, int exit_status) {}
	void warn (int level, const char *prefix, const char *fmt, va_list ap) {}
	void ask (int level, const char *prefix, const char *fmt, va_list ap) {}
	void put (const char *p) {}
	void put (const char *n, const char *p) {}
	void setio (class iofunctions *) {}
	void setonoff (int loglev, int value) {}
	const char *get_name () const { return nullptr; }
	const char *getprefix () const { return nullptr; }
	int getonoff (int level) const { return 0; }
	static void set_default_action (int loglev, int action) {}
	static int get_default_action (int loglev) { return 0; }
} logfunc_t;

