// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here
#include "d3d11.h"
#include "d3dcompiler.h"
#include <wrl/client.h>

// normally this isn't done in a header, but since this precompiled header (stdafx.h)
// is only used in this MSVC project, this "using" does not leak outside of this
// integration library
using Microsoft::WRL::ComPtr;
