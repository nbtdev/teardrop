/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RenderWindowD3D9.h"
#include "Gfx/Common.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

RenderWindow::RenderWindow(IDirect3DDevice9* device, HWND hWnd)
	: RenderTarget(device)
	, mHwnd(hWnd)
	, mInitFlags(0)
	, mSwapChain(0)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	mWidth = rect.right - rect.left;
	mHeight = rect.bottom - rect.top;
}

RenderWindow::RenderWindow(IDirect3DDevice9* device, HWND hWnd, int flags)
	: RenderTarget(device)
	, mHwnd(hWnd)
	, mInitFlags(0)
	, mSwapChain(0)
{
	mHwnd = hWnd;
	mInitFlags = flags;

	RECT rect;
	GetClientRect(hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	resize(width, height);
}

void RenderWindow::resize(int w, int h)
{
	assert(mHwnd);

	if (mSurface) {
		mSurface->Release();
		mSurface = 0;
	}

	if (mDepthStencil) {
		mDepthStencil->Release();
		mDepthStencil = 0;
	}

	mWidth = w;
	mHeight = h;

	ZeroMemory(&mPParams, sizeof(mPParams));
	mPParams.Windowed = TRUE;
	mPParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mPParams.BackBufferFormat = D3DFMT_UNKNOWN;
	mPParams.hDeviceWindow = mHwnd;
	mPParams.BackBufferWidth = mWidth;
	mPParams.BackBufferHeight = mHeight;
	mPParams.BackBufferCount = 1;
	mPParams.EnableAutoDepthStencil = TRUE;

	D3DFORMAT dsFmt = D3DFMT_UNKNOWN;

	if (mInitFlags & (INIT_ENABLE_DEPTH_BUFFER | INIT_ENABLE_STENCIL_BUFFER))
			dsFmt = D3DFMT_D24S8;

	mPParams.AutoDepthStencilFormat = dsFmt;

	mPParams.PresentationInterval = 
		(mInitFlags & INIT_ENABLE_VSYNC) ? (D3DPRESENT_DONOTWAIT | D3DPRESENT_INTERVAL_ONE) : D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = mDevice->CreateAdditionalSwapChain(&mPParams, &mSwapChain);

	if (SUCCEEDED(hr)) {
		// get render target surface ref
		mSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &mSurface);

		// create depth buffer for additional swap chain
		hr = mDevice->CreateDepthStencilSurface(
			mPParams.BackBufferWidth,
			mPParams.BackBufferHeight,
			mPParams.AutoDepthStencilFormat,
			D3DMULTISAMPLE_NONE, // todo: support AA at some point
			0,
			FALSE,
			&mDepthStencil,
			NULL);
	}

	assert(mSurface);
	assert(mDepthStencil);
}

RenderWindow::~RenderWindow()
{
}

HWND RenderWindow::hWnd()
{
	return mHwnd;
}

D3DPRESENT_PARAMETERS& RenderWindow::presentParams()
{
	return mPParams;
}

void RenderWindow::present()
{
	assert(mDevice);
	if (mDevice) {
		if (mSwapChain) {
			mSwapChain->Present(0, 0, 0, 0, 0);
		}
		else {
			mDevice->Present(0, 0, 0, 0);
		}
	}
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
