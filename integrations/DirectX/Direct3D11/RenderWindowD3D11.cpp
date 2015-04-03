/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RenderWindowD3D11.h"
#include "Gfx/Common.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

RenderWindow::RenderWindow(HWND hWnd)
	: RenderWindow(hWnd, 0)
{
}

RenderWindow::RenderWindow(HWND hWnd, int flags)
	: mHwnd(hWnd)
	, mInitFlags(flags)
{
	mHwnd = hWnd;

	RECT rect;
	GetClientRect(hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	resize(width, height);
}

void RenderWindow::resize(int w, int h)
{
	assert(mHwnd);

	//if (mSurface) {
	//	mSurface->Release();
	//	mSurface = 0;
	//}

	//if (mDepthStencil) {
	//	mDepthStencil->Release();
	//	mDepthStencil = 0;
	//}

	//if (mSwapChain) {
	//	mSwapChain->Release();
	//	mSwapChain = 0;
	//}

	mWidth = w;
	mHeight = h;

	//ZeroMemory(&mPParams, sizeof(mPParams));
	//mPParams.Windowed = TRUE;
	//mPParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//mPParams.BackBufferFormat = D3DFMT_UNKNOWN;
	//mPParams.hDeviceWindow = mHwnd;
	//mPParams.BackBufferWidth = mWidth;
	//mPParams.BackBufferHeight = mHeight;
	//mPParams.BackBufferCount = 1;
	//mPParams.EnableAutoDepthStencil = TRUE;

	//D3DFORMAT dsFmt = D3DFMT_UNKNOWN;

	//if (mInitFlags & (INIT_ENABLE_DEPTH_BUFFER | INIT_ENABLE_STENCIL_BUFFER))
	//	dsFmt = D3DFMT_D24S8;

	//mPParams.AutoDepthStencilFormat = dsFmt;

	//mPParams.PresentationInterval =
	//	(mInitFlags & INIT_ENABLE_VSYNC) ? (D3DPRESENT_DONOTWAIT | D3DPRESENT_INTERVAL_ONE) : D3DPRESENT_INTERVAL_IMMEDIATE;

	//HRESULT hr = mDevice->CreateAdditionalSwapChain(&mPParams, &mSwapChain);

	//if (SUCCEEDED(hr)) {
	//	// get render target surface ref
	//	mSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &mSurface);

	//	// create depth buffer for additional swap chain
	//	hr = mDevice->CreateDepthStencilSurface(
	//		mPParams.BackBufferWidth,
	//		mPParams.BackBufferHeight,
	//		mPParams.AutoDepthStencilFormat,
	//		D3DMULTISAMPLE_NONE, // todo: support AA at some point
	//		0,
	//		FALSE,
	//		&mDepthStencil,
	//		NULL);
	//}

	//assert(mSurface);
	//assert(mDepthStencil);
	//assert(mSwapChain);
}

RenderWindow::~RenderWindow()
{
	//if (mSwapChain)
	//	mSwapChain->Release();
}

HWND RenderWindow::hWnd()
{
	return mHwnd;
}

//D3DPRESENT_PARAMETERS& RenderWindow::presentParams()
//{
//	return mPParams;
//}

void RenderWindow::present()
{
	//assert(mDevice);
	//if (mDevice) {
	//	if (mSwapChain) {
	//		mSwapChain->Present(0, 0, 0, 0, 0);
	//	}
	//	else {
	//		mDevice->Present(0, 0, 0, 0);
	//	}
	//}
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
