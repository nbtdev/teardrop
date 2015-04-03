/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RenderWindowD3D11.h"
#include "RendererD3D11.h"
#include "Gfx/Common.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

RenderWindow::RenderWindow(Renderer* aRenderer, HWND hWnd)
	: RenderWindow(aRenderer, hWnd, 0)
{
}

RenderWindow::RenderWindow(Renderer* aRenderer, HWND hWnd, int flags)
	: mHwnd(hWnd)
	, mInitFlags(flags)
{
	assert(aRenderer);
	assert(mHwnd);

	mHwnd = hWnd;

	RECT rect;
	GetClientRect(hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	DXGI_SWAP_CHAIN_DESC desc = { 0 };
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;
	
	mDevice = aRenderer->device();
	IDXGIFactory* factory = aRenderer->factory();
	HRESULT hr = E_FAIL;

	if (factory) {
		hr = factory->CreateSwapChain(
			mDevice,
			&desc,
			&mSwapChain
			);
	}

	if (FAILED(hr) || !mSwapChain) {
		// TODO: throw a tantrum
	}
}

RenderWindow::~RenderWindow()
{
	if (mSwapChain)
		mSwapChain->Release();
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
