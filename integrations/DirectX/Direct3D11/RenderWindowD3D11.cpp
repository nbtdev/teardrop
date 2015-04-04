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
#include "Gfx/Exception.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

RenderWindow::RenderWindow(Renderer* aRenderer, HWND hWnd)
	: RenderWindow(aRenderer, hWnd, 800, 600, 0)
{
}

RenderWindow::RenderWindow(Renderer* aRenderer, HWND hWnd, int aFlags)
	: RenderWindow(aRenderer, hWnd, 800, 600, aFlags)
{
}

RenderWindow::RenderWindow(Renderer* aRenderer, HWND hWnd, int aWidth, int aHeight)
	: RenderWindow(aRenderer, hWnd, aWidth, aHeight, 0)
{
}

RenderWindow::RenderWindow(Renderer* aRenderer, HWND hWnd, int aWidth, int aHeight, int flags)
	: RenderTarget(aRenderer, aWidth, aHeight)
	, mHwnd(hWnd)
	, mInitFlags(flags)
{
	assert(aRenderer);
	assert(mHwnd);

	mHwnd = hWnd;

	RECT rect;
	GetClientRect(hWnd, &rect);

	// these typically will be the same as aWidth and aHeight, which are by now stored in mWidth and mHeight
	// but it's possible that the user for some reason wants different dimensions, so let them
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	DXGI_SWAP_CHAIN_DESC desc = { 0 };
	desc.BufferCount = 2;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;
	
	mDevice = aRenderer->device();

	assert(mDevice);
	if (!mDevice)
		throw InvalidParameterException("Invalid 'device' renderer property in Direct3D11::RenderWindow");

	ComPtr<IDXGIFactory> factory = aRenderer->factory();
	assert(factory);
	if (!factory)
		throw InvalidParameterException("Invalid 'factory' renderer property in Direct3D11::RenderWindow");

	HRESULT hr = E_FAIL;

	if (factory) {
		hr = factory->CreateSwapChain(
			mDevice.Get(),
			&desc,
			&mSwapChain
			);
	}

	if (FAILED(hr) || !mSwapChain)
		throw Exception("Could not create swap chain in Direct3D11::RenderWindow");

	hr = mSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		&mBackBuffer
		);

	if (FAILED(hr))
		throw Exception("Could not obtain back buffer from swap chain in Direct3D11::RenderWindow");

	hr = mDevice->CreateRenderTargetView(
		mBackBuffer.Get(),
		nullptr,
		&mRenderTargetView
		);

	if (FAILED(hr))
		throw Exception("Could not create render target view in Direct3D11::RenderWindow");

	mBackBuffer->GetDesc(&mDesc);
}

RenderWindow::~RenderWindow()
{
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
	
	HRESULT hr = mSwapChain->ResizeBuffers(
		0, // preserve the number of buffers
		mWidth, 
		mHeight, 
		DXGI_FORMAT_UNKNOWN, // preserve the existing format
		0
		);

	assert(SUCCEEDED(hr));

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
	if (mSwapChain) {
		mSwapChain->Present(0, 0);
	}
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
