/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxViewportD3D9.h"
#include "GfxRenderTarget.h"
#include "GfxCamera.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxViewportD3D9::GfxViewportD3D9()
{
}
//---------------------------------------------------------------------------
GfxViewportD3D9::~GfxViewportD3D9()
{
}
//---------------------------------------------------------------------------
bool GfxViewportD3D9::initialize(GfxRenderTarget* /*pRT*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxViewportD3D9::destroy()
{
	return true;
}
