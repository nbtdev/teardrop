/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ViewportD3D9.h"

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

Viewport::Viewport(Gfx::RenderTarget* rt)
	: Gfx::Viewport(rt)
{
	mVP.MinZ = 0;
	mVP.MaxZ = 1;
}

Viewport::~Viewport()
{
}

D3DVIEWPORT9& Viewport::viewport()
{
	updateDimensions();

	mVP.X = (DWORD)mPos.x;
	mVP.Y = (DWORD)mPos.y;
	mVP.Width = (DWORD)mSize.x;
	mVP.Height = (DWORD)mSize.y;

	return mVP;
}

} // namespace Direct3D9
} // Gfx
} // Teardrop
