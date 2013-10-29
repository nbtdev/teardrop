/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RenderTargetD3D9.h"
#include "ViewportD3D9.h"
#include "Math/Vector2.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

RenderTarget::RenderTarget(IDirect3DDevice9* device)
	: mDevice(device)
	, mSurface(0)
	, mDepthStencil(0)
	, mWidth(0)
	, mHeight(0)
{
	assert(mDevice);
}

RenderTarget::~RenderTarget()
{
	for (Viewports::iterator it = mViewports.begin(); it != mViewports.end(); ++it) {
		delete it->second;
	}
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

void RenderTarget::clear(
	bool color /* = true */, unsigned int clearColor /* = 0 */, 
	bool depth /* = true */, float depthValue /* = 1 */, 
	bool stencil /* = true */, unsigned int stencilValue /* = 0 */)
{
	assert(mDevice);
	if (!mDevice)
		return;

	DWORD flags = 0;
	if (color)
		flags |= D3DCLEAR_TARGET;
	if (depth)
		flags |= D3DCLEAR_ZBUFFER;
	if (stencil)
		flags |= D3DCLEAR_STENCIL;

	D3DCOLOR col = (D3DCOLOR)clearColor;

	if (flags)
		mDevice->Clear(0, NULL, flags, col, depthValue, stencilValue);
}

int RenderTarget::width()
{
	return mWidth;
}

int RenderTarget::height()
{
	return mHeight;
}

void RenderTarget::setCurrent()
{
	assert(mDevice);
	if (mDevice) {
		// TODO: support MRT
		mDevice->SetRenderTarget(0, mSurface);
	}
}

Gfx::Viewport* RenderTarget::addViewport(float x/* =0 */, float y/* =0 */, float w/* =1 */, float h/* =1 */, unsigned int zOrder/* =0 */)
{
	Viewport* vp = TD_NEW Viewport(this);

	vp->setPosition(Vector2(x, y), true);
	vp->setSize(Vector2(w, h), true);

	Viewports::value_type val(zOrder, vp);
	mViewports.insert(val);
	return vp;
}

void RenderTarget::releaseViewport(Gfx::Viewport* vp)
{
	for (Viewports::iterator it = mViewports.begin(); it != mViewports.end(); ++it) {
		if (it->second == vp) {
			mViewports.erase(it);
			delete vp;
			return;
		}
	}
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
