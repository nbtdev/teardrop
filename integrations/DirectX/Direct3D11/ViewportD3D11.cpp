/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ViewportD3D11.h"

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

Viewport::Viewport(Gfx::RenderTarget* rt)
	: Gfx::Viewport(rt)
{
	mVP.MinDepth = 0.f;
	mVP.MaxDepth = 1.f;
}

Viewport::~Viewport()
{
}

D3D11_VIEWPORT& Viewport::viewport()
{
	updateDimensions();

	mVP.TopLeftX = mPos.x;
	mVP.TopLeftY = mPos.y;
	mVP.Width = mSize.x;
	mVP.Height = mSize.y;

	return mVP;
}

} // namespace Direct3D9
} // Gfx
} // Teardrop
