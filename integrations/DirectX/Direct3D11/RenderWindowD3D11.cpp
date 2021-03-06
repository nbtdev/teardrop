/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
#include "RenderWindowD3D11.h"
#include "RendererD3D11.h"
#include "Gfx/Common.h"
#include "Gfx/Exception.h"
#include <assert.h>
#include <VersionHelpers.h>

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
	: RenderTarget(aRenderer->device(), aWidth, aHeight)
	, mHwnd(hWnd)
	, mInitFlags(flags)
{
	assert(mHwnd);

	mHwnd = hWnd;

	RECT rect;
	GetClientRect(hWnd, &rect);

	// these typically will be the same as aWidth and aHeight, which are by now stored in mWidth and mHeight
	// but it's possible that the user for some reason wants different dimensions, so let them
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	DXGI_SWAP_EFFECT effect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // Win8 and greater
	if (!IsWindows8OrGreater())
		effect = DXGI_SWAP_EFFECT_DISCARD; // Win7 and Vista, and possibly XP

	DXGI_SWAP_CHAIN_DESC desc = { 0 };
	desc.BufferCount = 2;
	desc.SwapEffect = effect;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

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
	mWidth = w;
	mHeight = h;

	mBackBuffer.Reset();
	assert(!mBackBuffer);

	mRenderTargetView.Reset();
	assert(!mRenderTargetView);

	mDepthStencilTexture.Reset();
	assert(!mDepthStencilTexture);

	mDepthStencilView.Reset();
	assert(!mDepthStencilView);

	HRESULT hr = mSwapChain->ResizeBuffers(
		0, // preserve the number of buffers
		mWidth, 
		mHeight, 
		DXGI_FORMAT_UNKNOWN, // preserve the existing format
		0
		);

	assert(SUCCEEDED(hr));

	hr = mSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		&mBackBuffer
		);

	if (FAILED(hr))
		throw Exception("Could not obtain back buffer from swap chain in Direct3D11::RenderWindow::resize");

	hr = mDevice->CreateRenderTargetView(
		mBackBuffer.Get(),
		nullptr,
		&mRenderTargetView
		);

	if (FAILED(hr))
		throw Exception("Could not create render target view in Direct3D11::RenderWindow::resize");

	hr = mDevice->CreateRenderTargetView(
		mBackBuffer.Get(),
		nullptr,
		&mRenderTargetView
		);

	if (FAILED(hr))
		throw Exception("Could not create render target view in Direct3D11::RenderWindow::resize");

	CD3D11_TEXTURE2D_DESC dsDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,		// tex format
		static_cast<UINT>(mWidth),
		static_cast<UINT>(mHeight),
		1,									// only one texture in the view
		1,									// single mipmap level
		D3D11_BIND_DEPTH_STENCIL
		);

	hr = mDevice->CreateTexture2D(
		&dsDesc,
		nullptr,	// no initial data
		&mDepthStencilTexture
		);

	if (FAILED(hr))
		throw Exception("Could not create depth stencil texture in Direct3D11::RenderWindow::resize");

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	hr = mDevice->CreateDepthStencilView(
		mDepthStencilTexture.Get(),
		&dsvDesc,
		&mDepthStencilView
		);

	if (FAILED(hr))
		throw Exception("Could not create depth stencil view in Direct3D11::RenderWindow::resize");
}

HWND RenderWindow::hWnd()
{
	return mHwnd;
}

void RenderWindow::present()
{
	if (mSwapChain) {
		HRESULT hr = mSwapChain->Present(0, 0);

		if (FAILED(hr)) {
			throw Exception("Failed to swap buffers");
		}
	}
}

} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
