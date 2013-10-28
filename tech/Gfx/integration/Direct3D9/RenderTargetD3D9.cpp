/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RenderTargetD3D9.h"

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

RenderTarget::RenderTarget(IDirect3DDevice9* device)
	: mDevice(device)
	, mSurface(0)
	, mDepthStencil(0)
{

}

RenderTarget::~RenderTarget()
{

}

IDirect3DSurface9* RenderTarget::surface()
{
	return mSurface;
}

IDirect3DSurface9* RenderTarget::depthStencil()
{
	return mDepthStencil;
}

void RenderTarget::setSurface(IDirect3DSurface9* surface)
{
	mSurface = surface;
}

void RenderTarget::setDepthStencil(IDirect3DSurface9* depthStencil)
{
	mDepthStencil = depthStencil;
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
