// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// TODO: reference additional headers your program requires here
#include "Config.h"
#define  _CRT_SECURE_NO_WARNINGS

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <rpc.h>
#include <process.h>
#include <limits.h>
#include <Shlwapi.h>
#include <shlobj.h>

#include <list>
#include <map>
#include <vector>
