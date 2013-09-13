#ifndef __STDAFX_H__
#define __STDAFX_H__


// Required for VS 2008 (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif



//#define FUNCTION_FIXED_POSITION() \
//	OutputDebugStringA("FUNC: "); \
//	OutputDebugStringA(__FUNCTION__); \
//	OutputDebugStringA("\n");


//for cef.
#include <algorithm>
#include <sstream>
#include <string>
#include <set>

#include <windows.h>
#include <objbase.h>
#include <cassert>
#include <Shlwapi.h>
#include "../DuiLib/UIlib.h"
#include "resource.h"



#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>


#include <objbase.h>

using namespace DuiLib;


#include "MiniDumper.h"
typedef CDuiString CString;

//for libcurl
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment (lib, "libcurl_imp.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "openldap.lib")
#pragma comment(lib, "libeay32.lib")

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "DbgHelp.lib")



#endif