/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "integration/Direct3D9/RendererD3D9.h"
#include "integration/Direct3D9/RenderTargetD3D9.h"
#include "integration/Direct3D9/RenderWindowD3D9.h"
#include "integration/Direct3D9/TextureManagerD3D9.h"
#include "integration/Direct3D9/ShaderManagerD3D9.h"
#include "integration/Direct3D9/BufferManagerD3D9.h"
#include "integration/Direct3D9/Integration.h"
#include "Util/_String.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

Renderer::Renderer()
	: mD3D9(0)
	, mDevice(0)
	, mDepthStencil(0)
	, mSwapChain(0)
	, mCurrentRT(0)
	, mCurrentCamera(0)
	, mCurrentVP(0)
{
}

Renderer::~Renderer()
{
	assert(mD3D9 == 0 && "Did you forget to call Renderer::shutdown()?");
}

Gfx::RenderTarget* Renderer::initialize(uintptr_t windowHandle, int flags)
{
	assert(windowHandle);
	if (!windowHandle)
		return 0;

	mD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	HWND hWnd = (HWND)windowHandle;
	RECT rect;
	GetClientRect(hWnd, &rect);

	if (!mD3D9) {
		return 0;
	}

	// The way D3D9 works is that it must have an HWND to create a device.
	// This means that there is an implicit "default render window" as the 
	// first render target, and it is attached/embedded into the supplied HWND

	// create device embedded in supplied window
	ZeroMemory(&mPParams, sizeof(mPParams));
	mPParams.BackBufferCount = 1;
	mPParams.Windowed = (flags & INIT_FULLSCREEN) ? FALSE : TRUE;
	mPParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mPParams.BackBufferFormat = D3DFMT_UNKNOWN;
	mPParams.hDeviceWindow = hWnd;
	mPParams.BackBufferWidth = (UINT)(rect.right - rect.left);
	mPParams.BackBufferHeight = (UINT)(rect.bottom - rect.top);

	if (flags & INIT_ENABLE_STENCIL_BUFFER) {
		mPParams.EnableAutoDepthStencil = TRUE;
		mPParams.AutoDepthStencilFormat = D3DFMT_D24S8;
	}

	mPParams.PresentationInterval = 
		(flags & INIT_ENABLE_VSYNC) ? (D3DPRESENT_DONOTWAIT | D3DPRESENT_INTERVAL_ONE) : D3DPRESENT_INTERVAL_IMMEDIATE;

	// find the adapter to use 
	UINT adapterIndex = D3DADAPTER_DEFAULT;
	D3DDEVTYPE devType = D3DDEVTYPE_HAL;

	if (flags & INIT_ENABLE_NVPERFHUD)
	{
		for (UINT a=0; a<mD3D9->GetAdapterCount(); ++a)
		{
			D3DADAPTER_IDENTIFIER9 id;
			HRESULT hr = mD3D9->GetAdapterIdentifier(a, 0, &id);

			if (SUCCEEDED(hr) && strstr(id.Description, "PerfHUD") != 0)
			{
				adapterIndex = a;
				devType = D3DDEVTYPE_REF;
				break;
			}
		}
	}

	HRESULT hr = S_OK;
	if (FAILED(hr=mD3D9->CreateDevice(
		adapterIndex,
		devType,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&mPParams,
		&mDevice)))
	{
		//pRend->logD3DError(hr);
		return 0;
	}

	// the first render target in the list is the default/implicit render target we just created, 
	// and it has to be a render window
	Direct3D9::RenderWindow* renderWindow = TD_NEW Direct3D9::RenderWindow(mDevice, mPParams, hWnd);
	mRenderTargets.push_back(renderWindow);

	IDirect3DSurface9* surface;
	mDevice->GetRenderTarget(0, &surface);
	renderWindow->setSurface(surface);
	mDevice->GetDepthStencilSurface(&surface);
	renderWindow->setDepthStencil(surface);

	// finally, set up the managers we need
	TD_NEW ShaderManager;
	TD_NEW BufferManager(mDevice);
	TD_NEW TextureManager;

	return renderWindow;
}

void Renderer::shutdown()
{
	if (mDevice) {
		mDevice->Release();
		mDevice = 0;
	}

	if (mD3D9) {
		mD3D9->Release();
		mD3D9 = 0;
	}

	TextureManager::shutdown();
	BufferManager::shutdown();
	ShaderManager::shutdown();
}

void Renderer::setRenderTarget(Gfx::RenderTarget* rt)
{
	// check to make sure it actually is one of ours
	for (size_t r=0; r<mRenderTargets.size(); ++r) {
		if (mRenderTargets[r] == rt) {
			if (rt != mCurrentRT) {
				rt->setCurrent();
				mCurrentRT = rt;
				return;
			}
		}
	}
}

Gfx::RenderTarget* Renderer::createRenderWindow(uintptr_t /*hWnd*/, SurfaceFormat /*fmt*/, int /*flags*/)
{
	return 0;
}

Gfx::RenderTarget* Renderer::createRenderTexture(int /*w*/, int /*h*/, SurfaceFormat /*fmt*/, int /*flags*/)
{
	return 0;
}

static Gfx::Renderer* create() {
	return TD_NEW Renderer();
}

void registerIntegration() 
{
	static RendererRegistration reg;
	if (!reg.mDisplayName) {
		reg.mUUID.fromString("3c04a9ef-fd2b-4074-bf1a-f4afd4fcbdbf");
		reg.mCreateFn = &create;
		reg.mDisplayName = "Direct3D 9";
		registerRenderer(&reg);
	}
}

void Renderer::beginFrame(
	bool color /* = true */, unsigned int clearColor /* = 0 */, 
	bool depth /* = true */, float depthValue /* = 1 */, 
	bool stencil /* = true */, unsigned int stencilValue /* = 0 */)
{
	assert(mCurrentRT);
	if (mCurrentRT) {
		mCurrentRT->clear(color, clearColor, depth, depthValue, stencil, stencilValue);
	}

	assert(mDevice);
	if (mDevice) {
		mDevice->BeginScene();
	}
}

void Renderer::beginScene(Camera* camera, Viewport* vp)
{
	mCurrentCamera = camera;
	mCurrentVP = vp;
}

void Renderer::endScene()
{

}

void Renderer::endFrame()
{
	assert(mDevice);
	if (mDevice) {
		mDevice->EndScene();
	}

	assert(mCurrentRT);
	if (mCurrentRT) {
		mCurrentRT->present();
	}
}

} // Direct3D9
} // Gfx
} // Teardrop
