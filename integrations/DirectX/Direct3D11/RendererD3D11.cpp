/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RendererD3D11.h"
#include "IndexBufferD3D11.h"
#include "RenderTargetD3D11.h"
#include "RenderWindowD3D11.h"
#include "VertexBufferD3D11.h"
#include "VertexDeclarationD3D11.h"
#include "VertexShaderD3D11.h"
#include "TextureManagerD3D11.h"
#include "ShaderManagerD3D11.h"
#include "BufferManagerD3D11.h"
#include "ViewportD3D11.h"
#include "Gfx/Camera.h"
#include "Gfx/Exception.h"
#include "Gfx/Material.h"
#include "Gfx/ShaderConstantTable.h"
#include "Gfx/ShaderConstant.h"
#include "Gfx/Submesh.h"
#include "Math/Matrix44.h"
#include "Util/_String.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
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
namespace Direct3D11 {

Renderer::Renderer(int flags)
{
	if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&mFactory))) {
		DXGI_ADAPTER_DESC desc;
		const int len = sizeof(desc.Description) / 2;
		char buf[len];

		// enumerate adapters
		UINT i = 0;
		IDXGIAdapter* adapter = nullptr;

		while (mFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND) {
			// for now, just use the first one found
			if (!mAdapter)
				mAdapter = adapter;

			adapter->GetDesc(&desc);
			adapter->Release();

			size_t nChar = 0;
			wcstombs_s(&nChar, buf, desc.Description, len);
			Environment::get().pLogger->logMessage(buf);
			++i;
		}
	} else {
		throw Exception("Could not create D3D11 DXGI factory object");
	}

	UINT D3Dflags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	D3Dflags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL supported;

	HRESULT hr = D3D11CreateDevice(
		mAdapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3Dflags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&mDevice,
		&supported,
		&mDeviceContext
		);

	if (FAILED(hr)) {
		DXGI_ADAPTER_DESC desc;
		mAdapter->GetDesc(&desc);

		const int LEN = sizeof(desc.Description) / 2;
		char buf[LEN];
		size_t sz;
		wcstombs_s(&sz, buf, desc.Description, LEN);

		String msg("Could not create D3D11 hardware-accelerated device on adapter: ");
		msg += buf;

		throw Exception(msg);
	}

	TD_NEW BufferManager(this);
	TD_NEW TextureManager(mDevice);
	TD_NEW ShaderManager(mDevice);

	// ensure that the xform constants buffer struct is 16-byte-aligned (sanity check)
	assert(sizeof(mXformConstants) % 16 == 0);

	// create/initialize the constant buffer for the xforms
	mXformConstants.mViewIXf = Matrix44::IDENTITY;
	mXformConstants.mViewProj = Matrix44::IDENTITY;
	mXformConstants.mWorldInv = Matrix44::IDENTITY;
	mXformConstants.mWorldITXf = Matrix44::IDENTITY;
	mXformConstants.mWorldXf = Matrix44::IDENTITY;
	mXformConstants.mWvpXf = Matrix44::IDENTITY;

	CD3D11_BUFFER_DESC bufDesc(
		sizeof(mXformConstants),
		D3D11_BIND_CONSTANT_BUFFER
		);

	D3D11_SUBRESOURCE_DATA bufData = { 0 };
	bufData.pSysMem = &mXformConstants;

	hr = mDevice->CreateBuffer(
		&bufDesc,
		&bufData,
		&mXformConstantBuffer
		);

	if (FAILED(hr)) {
		throw Exception("Could not create xform constants buffer in Direct3D11::Renderer");
	}

	// create/initialize a matrix palette constant buffer, of the full 1024-"bone"
	// allotment; it sound inefficient but we can't update constant buffers partially,
	// so the plan is to make this have a non-negligible performance impact before we
	// "solve" the problem by keeping many different just-large-enough sized buffers
	// on the GPU

	// each row is one constant, and 4 rows per matrix, with 4096 constant-per-buffer limit
	bufDesc.ByteWidth = sizeof(Matrix44) * 1024; 

	hr = mDevice->CreateBuffer(
		&bufDesc,
		nullptr,
		&mMatrixPalette
		);

	if (FAILED(hr)) {
		throw Exception("Could not create matrix palette buffer in Direct3D11::Renderer");
	}

}

Renderer::~Renderer()
{
	ShaderManager::instance().shutdown();
	TextureManager::instance().shutdown();
	BufferManager::instance().shutdown();
}

#if 0
void Renderer::initialize()
{
	mD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

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
		for (UINT a = 0; a<mD3D9->GetAdapterCount(); ++a)
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
	if (FAILED(hr = mD3D9->CreateDevice(
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
	//TextureManager::shutdown();
	//BufferManager::shutdown();
	//ShaderManager::shutdown();

	//if (mDevice) {
	//	mDevice->Release();
	//	mDevice = 0;
	//}

	//if (mD3D9) {
	//	mD3D9->Release();
	//	mD3D9 = 0;
	//}
}

void Renderer::setRenderTarget(Gfx::RenderTarget* rt)
{
	// let user clear current RT if they wish
	if (rt == 0) {
		mCurrentRT = 0;
		return;
	}

	// check to make sure it actually is one of ours
	for (size_t r = 0; r<mRenderTargets.size(); ++r) {
		if (mRenderTargets[r] == rt) {
			if (rt != mCurrentRT) {
				rt->setCurrent();
				mCurrentRT = rt;
				return;
			}
		}
	}
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

#endif

std::shared_ptr<Gfx::RenderTarget> Renderer::createRenderWindow(uintptr_t hWnd, SurfaceFormat /*fmt*/, int flags)
{
	// RenderWindow will throw if failed, so let it bubble up
	HWND hwnd = (HWND)hWnd;
	RECT rect;
	GetClientRect(hwnd, &rect);

	std::shared_ptr<Gfx::RenderTarget> rtn(TD_NEW RenderWindow(this, (HWND)hWnd, rect.right-rect.left, rect.bottom-rect.top, flags));
	mRenderTargets.push_back(rtn);
	return rtn;
}

std::shared_ptr<Gfx::RenderTarget> Renderer::createRenderTexture(int /*w*/, int /*h*/, SurfaceFormat /*fmt*/, int /*flags*/)
{
	return nullptr;
}

static Gfx::Renderer* create(int flags) {
	return TD_NEW Renderer(flags);
}

void registerIntegration()
{
	static RendererRegistration reg;
	if (!reg.mDisplayName) {
		reg.mUUID.fromString("78fff834-587b-4315-9454-506b5d108dfc");
		reg.mCreateFn = &create;
		reg.mDisplayName = "Direct3D 11";
		registerRenderer(&reg);
	}
}

void Renderer::beginFrame()
{
}

void Renderer::beginScene(Camera* aCamera, Gfx::Viewport* aVP)
{
	mCurrentCamera = aCamera;

	// ensure camera has the correct aspect ratio
	mCurrentCamera->update();

	// update camera-related shader constants
	mCurrentCamera->getViewMatrix().invert(mXformConstants.mViewIXf);
	
	mXformConstants.mViewProj = mCurrentCamera->getViewProjMatrix();

	Viewport* vp = static_cast<Viewport*>(aVP);
	mDeviceContext->RSSetViewports(1, &vp->viewport());
}

void Renderer::beginObject(const Matrix44& worldXf)
{
	// set world xform
	mXformConstants.mWorldXf = worldXf;

	// and its derivatives
	worldXf.invert(mXformConstants.mWorldInv);
	mXformConstants.mWorldInv.transpose(mXformConstants.mWorldITXf);
	
	Matrix44 tmp = worldXf * mXformConstants.mViewProj;
	tmp.transpose(mXformConstants.mWvpXf);

	mDeviceContext->UpdateSubresource(
		mXformConstantBuffer.Get(),
		0,
		nullptr,
		&mXformConstants,
		0,
		0
		);

	// apply constants
	// TODO: apply matrix palette too
	mDeviceContext->VSSetConstantBuffers(
		0, 
		1, 
		mXformConstantBuffer.GetAddressOf()
		);
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

static D3D_PRIMITIVE_TOPOLOGY sD3DPrimTypes[] = {
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,    // PT_UNKNOWN
	D3D_PRIMITIVE_TOPOLOGY_POINTLIST,       // PT_POINT,
	D3D_PRIMITIVE_TOPOLOGY_LINELIST,        // PT_LINES,
	D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,       // PT_LINE_STRIP,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,    // PT_TRIANGLES,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,   // PT_TRIANGLE_STRIP,
};

void Renderer::render(Submesh* submesh)
{
	assert(mDeviceContext);
	if (!mDeviceContext)
		return;

	assert(submesh);
	if (!submesh)
		return;

	// vertex declaration (D3D11: input format)
	VertexDeclaration* decl = static_cast<VertexDeclaration*>(submesh->vertexDeclaration());
	assert(decl);

	if (decl)
		mDeviceContext->IASetInputLayout(decl->layout());

	// vertex shader
	VertexShader* vs = static_cast<VertexShader*>(ShaderManager::instance().createOrFindInstanceOf(submesh));
	vs->apply();

	// vertex buffers
	ID3D11Buffer* vbs[16];
	UINT strides[16];
	UINT offsets[16];
	int nVB = submesh->vertexBufferCount();
	for (int i=0; i<nVB && i<sizeof(vbs); ++i) {
		VertexBuffer* vb = static_cast<VertexBuffer*>(submesh->vertexBuffer(i));
		vbs[i] = vb->buffer();
		strides[i] = vb->vertexSize();
		offsets[i] = 0;
	}

	mDeviceContext->IASetVertexBuffers(
		0,
		nVB,
		vbs,
		strides,
		offsets);

	// index buffer
	DXGI_FORMAT sz = DXGI_FORMAT_R16_UINT;
	IndexBuffer* ib = static_cast<IndexBuffer*>(submesh->indexBuffer());
	if (ib->indexSize() == 4)
		sz = DXGI_FORMAT_R32_UINT;

	mDeviceContext->IASetIndexBuffer(
		ib->buffer(),
		sz,
		0);

	// primitive type
	mDeviceContext->IASetPrimitiveTopology(sD3DPrimTypes[submesh->primitiveType()]);

	// draw primitives
	mDeviceContext->DrawIndexed(
		ib->indexCount(),
		0,
		0);
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
}

void Renderer::updateMatrixPalette(const Matrix44* aMatrixPalette, int nMatrices)
{
	assert(mDeviceContext);
	if (!mDeviceContext)
		return;

	if (mMatrixPalette) {
		assert(nMatrices < 1395 && "Can only update a matrix palette of 1395 matrices or fewer");

		mDeviceContext->UpdateSubresource(
			mMatrixPalette.Get(),
			0,
			nullptr,
			aMatrixPalette,
			0,
			0
			);
	}
}

ComPtr<ID3D11Device> Renderer::device()
{
	return mDevice;
}

ComPtr<ID3D11DeviceContext> Renderer::context()
{
	return mDeviceContext;
}

ComPtr<IDXGIFactory> Renderer::factory()
{
	return mFactory;
}

} // Direct3D11
} // Gfx
} // Teardrop
