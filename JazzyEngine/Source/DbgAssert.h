// Defines a custom assertion macro
#pragma once
#include <intrin.h>

#ifdef _DEBUG
extern bool DbgAssertFunction(bool expr, const wchar_t* expr_string, const wchar_t* desc, int line_num, const wchar_t* file_name);

// These macros convert __FILE__ from char* to wchar_t*
#define DBG_WIDEN2(x) L##x
#define DBG_WIDEN(x) DBG_WIDEN2(x)
#define __WFILE__ DBG_WIDEN(__FILE__)

#define DbgAssert(expr, description) {if (DbgAssertFunction((expr), L#expr, L##description, __LINE__, __WFILE__)) {__debugbreak();}}
#else
#define DbgAssert(expr, description)
#endif // _DEBUG
