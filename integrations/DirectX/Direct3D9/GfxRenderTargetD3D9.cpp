/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxRenderTargetD3D9.h"
#include "Util/Hash.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
const unsigned __int64 GfxRenderTargetD3D9::IID = 
	hashString64("GfxRenderTargetD3D9");
//---------------------------------------------------------------------------
GfxRenderTargetD3D9::GfxRenderTargetD3D9()
{
}
