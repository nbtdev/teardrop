/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDSTABLEHEADERSD3D9_INCLUDED)
#define TDSTABLEHEADERSD3D9_INCLUDED

#if defined(USE_D3D9)
#include <windows.h>
#include <process.h>

#include <string>
#include <list>
#include <map>
#include <algorithm>

#if !defined(D3D_DEBUG_INFO)
#define D3D_DEBUG_INFO
#endif 

#include "d3d9.h"
#include "d3dx9.h"
#include "dxerr.h"
#endif // USE_D3D9

#endif // TDSTABLEHEADERSD3D9_INCLUDED
