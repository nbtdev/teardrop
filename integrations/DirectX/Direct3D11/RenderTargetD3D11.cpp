/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RenderTargetD3D11.h"
#include "RendererD3D11.h"
#include "ViewportD3D11.h"
#include "Gfx/Exception.h"
#include "Math/Vector2.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

RenderTarget::RenderTarget(Renderer* aRenderer, int aWidth, int aHeight)
	: mWidth(aWidth)
	, mHeight(aHeight)
{
	assert(aRenderer);
	if (aRenderer) {
		// create new depth-stencil and blend states for this RT
		mDevice = aRenderer->device();
		assert(mDevice);

		if (!mDevice) 
			throw InvalidParameterException("Invalid 'device' renderer property in Direct3D11::RenderTarget");

		mDeviceContext = aRenderer->context();
		assert(mDeviceContext);

		if (!mDeviceContext) 
			throw InvalidParameterException("Invalid 'deviceContext' renderer property in Direct3D11::RenderTarget");

		// create a blend state for single RT, with "normal" alpha blending enabled
		D3D11_BLEND_DESC desc = { 0 };
		desc.AlphaToCoverageEnable = FALSE;
		desc.IndependentBlendEnable = FALSE;
		desc.RenderTarget[0].BlendEnable = TRUE;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

		HRESULT hr = mDevice->CreateBlendState(
			&desc,
			&mBlendState
			);

		if (FAILED(hr) || !mBlendState)
			throw Exception("Could not create blend state object in Direct3D11::RenderTarget");

		// and now the depth/stencil
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
			throw Exception("Could not create depth stencil texture in Direct3D11::RenderTarget");

		CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

		hr = mDevice->CreateDepthStencilView(
			mDepthStencilTexture.Get(),
			&dsvDesc,
			&mDepthStencilView
			);

		if (FAILED(hr))
			throw Exception("Could not create depth stencil view in Direct3D11::RenderTarget");
	}
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::clear(
	bool color /* = true */, unsigned int clearColor /* = 0 */,
	bool depth /* = true */, float depthValue /* = 1 */,
	bool stencil /* = true */, unsigned int stencilValue /* = 0 */)
{
	assert(mDeviceContext);
	if (!mDeviceContext)
		return;

	if (color) {
		// break up color into 4 floats
		float floats[4];
		floats[3] = float(uint8_t(clearColor >> 24)) / 255.0f;
		floats[2] = float(uint8_t(clearColor >> 16)) / 255.0f;
		floats[1] = float(uint8_t(clearColor >> 8)) / 255.0f;
		floats[0] = float(uint8_t(clearColor)) / 255.0f;

		mDeviceContext->ClearRenderTargetView(
			mRenderTargetView.Get(), 
			floats
			);
	}

	if (depth || stencil) {
		UINT clearFlags = 0;
		if (depth) clearFlags |= D3D11_CLEAR_DEPTH;
		if (stencil) clearFlags |= D3D11_CLEAR_STENCIL;

		mDeviceContext->ClearDepthStencilView(
			mDepthStencilView.Get(), 
			clearFlags, 
			depthValue, 
			(UINT8)stencilValue
			);
	}
}

float RenderTarget::aspect()
{
	return float(mWidth) / float(mHeight);
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
	assert(mDeviceContext);
	if (mDeviceContext) {
		if (mBlendState) mDeviceContext->OMSetBlendState(mBlendState.Get(), NULL, 0xFFFFFFFF);
		if (mDepthStencilState) mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	}
}

void RenderTarget::unsetCurrent()
{
#if 0
	assert(mDevice);
	if (mDevice) {
		// TODO: support MRT
		mDevice->SetRenderTarget(0, nullptr);
	}
#endif
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
