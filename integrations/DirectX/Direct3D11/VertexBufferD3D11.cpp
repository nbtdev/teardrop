/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VertexBufferD3D11.h"
#include "Gfx/Exception.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

VertexBuffer::VertexBuffer(ComPtr<ID3D11Device> aDevice, Submesh* aParent)
	: Gfx::VertexBuffer(aParent)
	, mDevice(aDevice)
{
	assert(mDevice);
	ZeroMemory(&mSR, sizeof(mSR));
}

VertexBuffer::~VertexBuffer()
{
	if (mSR.pData) unmap();
}

void VertexBuffer::initialize(int aVertexCount, int aInitFlags, void* aData/* =0 */)
{
	D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
	UINT cpuFlag = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	if (aInitFlags & INIT_WRITEONLY) {
		cpuFlag = D3D11_CPU_ACCESS_WRITE;
	}

	if (aInitFlags & INIT_DYNAMIC) {
		usage = D3D11_USAGE_DYNAMIC;
	}

	if (aInitFlags & INIT_STATIC) {
		// if we have data, we can create as IMMUTABLE; otherwise, DEFAULT so that CPU access flags can be set
		if (aData) {
			usage = D3D11_USAGE_IMMUTABLE;
			cpuFlag = 0; // no CPU access flags allowed with IMMUTABLE
		}
		else {
			usage = D3D11_USAGE_DYNAMIC;
		}
	}

	int buflen = aVertexCount * vertexSize();

	CD3D11_BUFFER_DESC desc(
		buflen,
		D3D11_BIND_VERTEX_BUFFER,
		usage,
		cpuFlag
		);

	D3D11_SUBRESOURCE_DATA sd = { 0 };
	D3D11_SUBRESOURCE_DATA* psd = nullptr;

	if (aData) {
		sd.pSysMem = aData;
		psd = &sd;
	}

	HRESULT hr = mDevice->CreateBuffer(
		&desc,
		psd,
		&mD3D11Buffer
		);

	assert(SUCCEEDED(hr));
	if (FAILED(hr)) {
		throw Exception("Could not create D3D11 vertex buffer in VertexBuffer::initialize");
	}
	else {
		mInitFlags = aInitFlags;
		mCount = aVertexCount;
	}
}

void VertexBuffer::resize(int aVertexCount)
{
	mD3D11Buffer.Reset();

	initialize(mCount, mInitFlags);
}

void* VertexBuffer::map(MapFlags flags /*=MAP_ANY*/)
{
	assert(!mSR.pData);
	if (mSR.pData) {
		throw BufferMappedException("Vertex buffer already mapped", mSR.pData);
	}

	ComPtr<ID3D11DeviceContext> ctx;
	mDevice->GetImmediateContext(&ctx);

	// if this is D3D10 and we have to read, then we need to copy data to a
	// staging buffer (possibly creating that first if necessary), then copying
	// from that staging buffer to CPU
	D3D_FEATURE_LEVEL level = mDevice->GetFeatureLevel();
	if (level < D3D_FEATURE_LEVEL_11_0 && flags == MAP_READONLY) {
		if (!mD3D11StagingBuffer) {
			int buflen = mCount * vertexSize();

			CD3D11_BUFFER_DESC desc(
				buflen,
				0,
				D3D11_USAGE_STAGING,
				D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE
				);

			HRESULT hr = mDevice->CreateBuffer(
				&desc,
				nullptr,
				&mD3D11StagingBuffer
				);

			assert(SUCCEEDED(hr));
			if (FAILED(hr)) {
				throw Exception("Could not create D3D11 staging buffer in VertexBuffer::map");
			}
		}

		// then copy the original resource to the staging buffer...
		ctx->CopyResource(mD3D11StagingBuffer.Get(), mD3D11Buffer.Get());

		// and then map the staging buffer and return that
		HRESULT hr = ctx->Map(
			mD3D11StagingBuffer.Get(), 
			0, 
			D3D11_MAP_READ, 
			0, 
			&mSR);

		if (SUCCEEDED(hr)) {
			mMappedStaging = true;
			return mSR.pData;
		}
	} else {
		// any other combinations of levels and flags should work fine 

		D3D11_MAP D3DFlags;

		// TODO: if we allow dynamic flags on create then we can lock discard

		if (flags & MAP_DISCARD)
			D3DFlags = D3D11_MAP_WRITE_DISCARD;

		// TODO: D3D gets upset if you try to lock a write-only buffer as read-only...

		if (flags & MAP_READONLY)
			D3DFlags = D3D11_MAP_READ;

		if (mD3D11Buffer && mDevice) {
			HRESULT hr = ctx->Map(
				mD3D11Buffer.Get(),
				0,
				D3DFlags,
				0,
				&mSR);

			if (SUCCEEDED(hr)) {
				mMappedStaging = false;
				return mSR.pData;
			}
		}
	}

	return nullptr;
}

void VertexBuffer::unmap()
{
	assert(mSR.pData);

	if (mDevice && mSR.pData) {
		ComPtr<ID3D11DeviceContext> ctx;
		mDevice->GetImmediateContext(&ctx);

		if (mMappedStaging)
			ctx->Unmap(mD3D11StagingBuffer.Get(), 0);
		else
			ctx->Unmap(mD3D11Buffer.Get(), 0);

		mMappedStaging = false;

		ZeroMemory(&mSR, sizeof(mSR));
	}
}

ID3D11Buffer* VertexBuffer::buffer()
{
	return mD3D11Buffer.Get();
}

} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
