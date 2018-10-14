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
#include "RendererD3D9.h"
#include "RenderTargetD3D9.h"
#include "RenderWindowD3D9.h"
#include "ViewportD3D9.h"
#include "FragmentShaderD3D9.h"
#include "VertexShaderD3D9.h"
#include "IndexBufferD3D9.h"
#include "VertexBufferD3D9.h"
#include "VertexDeclarationD3D9.h"
#include "Texture2DD3D9.h"
#include "TextureManagerD3D9.h"
#include "ShaderManagerD3D9.h"
#include "BufferManagerD3D9.h"
#include "Gfx/Camera.h"
#include "Gfx/Material.h"
#include "Gfx/ShaderConstantTable.h"
#include "Gfx/ShaderConstant.h"
#include "Gfx/Submesh.h"
#include "Math/Matrix44.h"
#include "Util/_String.h"
#include <assert.h>

static const char* WORLDINVTRANS = "WorldITXf";
static const char* WORLDVIEWPROJ = "WvpXf";
static const char* WORLD = "WorldXf";
static const char* WORLDINV = "WorldInv";
static const char* VIEWINV = "ViewIXf";
static const char* VIEWPROJ = "ViewProj"; 
static const char* MATRIXPALETTE = "Bones";

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
	, mWorldITXf(0)
	, mWvpXf(0)
	, mWorldXf(0)
	, mWorldInv(0)
	, mViewIXf(0)
	, mViewProj(0)
	, mBones(0)
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
	Direct3D9::RenderWindow* renderWindow = TD_NEW Direct3D9::RenderWindow(mDevice, hWnd);
	mRenderTargets.push_back(renderWindow);

	IDirect3DSurface9* surface;
	mDevice->GetRenderTarget(0, &surface);
	renderWindow->setSurface(surface);
	mDevice->GetDepthStencilSurface(&surface);
	renderWindow->setDepthStencil(surface);

	// finally, set up the managers we need
	TD_NEW ShaderManager(mDevice);
	TD_NEW BufferManager(mDevice);
	TD_NEW TextureManager(mDevice);

	// populate shader constant table
	ShaderConstantTable* tab = ShaderManager::instance().constantTable();
	/*
	"float4x4 WorldITXf : WORLDINVTRANS;\n"
	"float4x4 WvpXf : WORLDVIEWPROJ;\n"
	"float4x4 WorldXf : WORLD;\n"
	"float4x4 WorldInv : WORLDINV;\n"
	"float4x4 ViewIXf : VIEWINV;\n"
	"float4x4 ViewProj : VIEWPROJ;\n"
	"float4 Bones[208] : MATRIXPALETTE;\n"	*/

	// TODO: add these to a semantic-based lookup table?
	mWorldITXf = tab->addNew(WORLDINVTRANS, VET_FLOAT, 4, 4);
	mWvpXf = tab->addNew(WORLDVIEWPROJ, VET_FLOAT, 4, 4);
	mWorldXf = tab->addNew(WORLD, VET_FLOAT, 4, 4);
	mWorldInv = tab->addNew(WORLDINV, VET_FLOAT, 4, 4);
	mViewIXf = tab->addNew(VIEWINV, VET_FLOAT, 4, 4);
	mViewProj = tab->addNew(VIEWPROJ, VET_FLOAT, 4, 4);
	mBones = tab->addNew(MATRIXPALETTE, VET_FLOAT, 4, 208);

	return renderWindow;
}

void Renderer::shutdown()
{
	TextureManager::shutdown();
	BufferManager::shutdown();
	ShaderManager::shutdown();

	if (mDevice) {
		mDevice->Release();
		mDevice = 0;
	}

	if (mD3D9) {
		mD3D9->Release();
		mD3D9 = 0;
	}
}

void Renderer::setRenderTarget(Gfx::RenderTarget* rt)
{
	// let user clear current RT if they wish
	if (rt == 0) {
		mCurrentRT = 0;
		return;
	}

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

Gfx::RenderTarget* Renderer::createRenderWindow(uintptr_t hWnd, SurfaceFormat /*fmt*/, int flags)
{
	Gfx::RenderTarget* rt = TD_NEW RenderWindow(mDevice, (HWND)hWnd, flags);
	mRenderTargets.push_back(rt);
	return rt;
}

Gfx::RenderTarget* Renderer::createRenderTexture(int /*w*/, int /*h*/, SurfaceFormat /*fmt*/, int /*flags*/)
{
	return 0;
}

void Renderer::releaseRenderTarget(Gfx::RenderTarget* rt)
{
	// don't release current render target
	// TODO: warn about this in a log or something
	assert(rt != mCurrentRT);
	if (rt != mCurrentRT) {
		// find it and erase it from the RT list
		for (RenderTargets::iterator it = mRenderTargets.begin(); it != mRenderTargets.end(); ++it) {
			if (rt == *it) {
				mRenderTargets.erase(it);
				break;
			}
		}

		// delete it regardless
		delete rt;
	}
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

void Renderer::beginScene(Camera* camera, Gfx::Viewport* vp)
{
	assert(mCurrentRT);

	mCurrentCamera = camera;
	mCurrentVP = vp;

	// ensure camera has the correct aspect ratio
	camera->setAspect(float(mCurrentRT->width()) / float(mCurrentRT->height()));
	camera->update();

	// update camera-related shader constants
	Matrix44 tmp;
	camera->getViewMatrix().invert(tmp);
	mViewIXf->set(&tmp);
	mViewProj->set(&camera->getViewProjMatrix());

	if (mCurrentVP) {
		Viewport* d3d9VP = static_cast<Viewport*>(mCurrentVP);
		mDevice->SetViewport(&d3d9VP->viewport());
	}
	else {
		// default to full-RT viewport
		D3DVIEWPORT9 vpt;
		vpt.MinZ = 0;
		vpt.MaxZ = 1;
		vpt.X = 0;
		vpt.Y = 0;
		vpt.Width = mCurrentRT->width();
		vpt.Height = mCurrentRT->height();
		mDevice->SetViewport(&vpt);
	}
}

void Renderer::beginObject(const Matrix44& worldXf)
{
	// set world xform
	mWorldXf->set(&worldXf);

	// and its derivatives
	Matrix44 tmp, tmp2;
	worldXf.invert(tmp);
	mWorldInv->set(&tmp);

	tmp.transpose(tmp2);
	mWorldITXf->set(&tmp2);

	tmp = worldXf * *((const Matrix44*)mViewProj->data());
	tmp.transpose(tmp2);
	mWvpXf->set(&tmp2);
}

void Renderer::apply(Material* material)
{
	if (material) 
		material->apply();
}

/*
enum PrimitiveType
{
PT_UNKNOWN = 0,
PT_POINTLIST,
PT_LINELIST,
PT_LINESTRIP,
PT_TRILIST,
PT_TRISTRIP,
PT_TRIFAN,
};
*/

static D3DPRIMITIVETYPE sD3DPrimTypes [] = {
	D3DPT_TRIANGLELIST,    // PT_UNKNOWN
	D3DPT_POINTLIST,       // PT_POINT,
	D3DPT_LINELIST,		   // PT_LINES,
	D3DPT_LINESTRIP,	   // PT_LINE_STRIP,
	D3DPT_TRIANGLELIST,	   // PT_TRIANGLES,
	D3DPT_TRIANGLESTRIP,   // PT_TRIANGLE_STRIP,
};

void Renderer::render(Submesh* submesh)
{
	assert(submesh);
	if (!submesh)
		return;

	// vertex declaration
	VertexDeclaration* decl = static_cast<VertexDeclaration*>(submesh->vertexDeclaration());
	mDevice->SetVertexDeclaration(decl->declaration());

	// vertex buffer(s)
	int nVB = submesh->vertexBufferCount();
	int nVerts = 0;
	for (int v=0; v<nVB; ++v) {
		VertexBuffer* vb = static_cast<VertexBuffer*>(submesh->vertexBuffer(v));
		mDevice->SetStreamSource((UINT)v, vb->buffer(), 0, (UINT)vb->vertexSize());

		// all vertex buffers in the submesh must have the same vertex count
		nVerts = vb->vertexCount();
	}

	// index buffer
	IndexBuffer* ib = static_cast<IndexBuffer*>(submesh->indexBuffer());

	// vertex shader
	VertexShader* vs = static_cast<VertexShader*>(ShaderManager::instance().createOrFindInstanceOf(submesh));
	vs->apply();

	// draw primitives
	if (ib) {
		// indexed primitives
		mDevice->SetIndices(ib->buffer());

		// TODO: support other than trilist?
		int primitiveCount = ib->indexCount() / 3;

		mDevice->DrawIndexedPrimitive(
			sD3DPrimTypes[submesh->primitiveType()],
			0, // offset into vertex buffer
			0, // min vertex index relative to vertex offset
			(UINT)nVerts,
			0, // offset into index buffer
			(UINT)primitiveCount
			);
	}
	else {
		// non-indexed primitives

		// TODO: if non-indexed mode is even supported, support other 
		// than trilists
		int primitiveCount = nVerts / 3;

		mDevice->DrawPrimitive(
			D3DPT_TRIANGLELIST,
			0, // vertex offset
			(UINT)primitiveCount
			);
	}
}

void Renderer::endObject()
{

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
