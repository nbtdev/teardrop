// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define D3D_DEBUG_INFO
#include <windows.h>
#include <process.h>

#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>

//#if !defined(D3D_DEBUG_INFO)
//#define D3D_DEBUG_INFO
//#endif 

#include "d3d9helper.h"
#include "d3d.h"
//#include "d3dx9.h"
//#include "dxerr.h"
