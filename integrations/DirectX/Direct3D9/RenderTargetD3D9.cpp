/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
	if (mDevice)
		mDevice->AddRef();
}

RenderTarget::~RenderTarget()
{
	for (Viewports::iterator it = mViewports.begin(); it != mViewports.end(); ++it) {
		delete it->second;
	}

	if (mSurface)
		mSurface->Release();

	if (mDepthStencil)
		mDepthStencil->Release();

	if (mDevice)
		mDevice->Release();
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

void RenderTarget::unsetCurrent()
{
	assert(mDevice);
	if (mDevice) {
		// TODO: support MRT
		mDevice->SetRenderTarget(0, nullptr);
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
