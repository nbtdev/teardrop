/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "IndexBufferD3D11.h"
#include "Gfx/Exception.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

	IndexBuffer::IndexBuffer(ComPtr<ID3D11Device> aDevice, Submesh* aParent)
		: Gfx::IndexBuffer(aParent)
		, mDevice(aDevice)
	{
		assert(mDevice);
		ZeroMemory(&mSR, sizeof(mSR));
	}

IndexBuffer::~IndexBuffer()
{
	if (mSR.pData) unmap();
}

void IndexBuffer::initialize(int indexCount, int aInitFlags, void* aData/* =0 */)
{
	int indexSize = 2;
	if (indexCount > 65535) {
		indexSize = 4;
	}

	D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
	UINT cpuFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	if (aInitFlags & INIT_WRITEONLY)
		cpuFlags = D3D11_CPU_ACCESS_WRITE;

	if (aInitFlags & INIT_DYNAMIC)
		usage = D3D11_USAGE_DYNAMIC;

	if (aInitFlags & INIT_STATIC) {
		// D3D10 will need to be dynamic if we do not supply the data right now...
		D3D_FEATURE_LEVEL level = mDevice->GetFeatureLevel();
		if (level < D3D_FEATURE_LEVEL_11_0 && !aData) {
			usage = D3D11_USAGE_DYNAMIC;
		} else {
			usage = D3D11_USAGE_IMMUTABLE;
			cpuFlags = 0;
		}
	}

	CD3D11_BUFFER_DESC desc(
		indexSize * indexCount,
		D3D11_BIND_INDEX_BUFFER,
		usage,
		cpuFlags
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

	if (FAILED(hr)) {
		throw Exception("Could not create D3D11 index buffer in IndexBuffer::initialize");
	}
	else {
		mSize = indexSize;
		mCount = indexCount;
		mInitFlags = aInitFlags;
	}
}

void IndexBuffer::resize(int indexCount)
{
	if (mD3D11Buffer)
		mD3D11Buffer.Reset();

	initialize(indexCount, mInitFlags);
}

void* IndexBuffer::map(MapFlags flags /*=MAP_ANY*/)
{
	assert(!mSR.pData);
	if (mSR.pData) {
		throw BufferMappedException("Index buffer already mapped", mSR.pData);
	}

	ComPtr<ID3D11DeviceContext> ctx;
	mDevice->GetImmediateContext(&ctx);

	// if this is D3D10 and we have to read, then we need to copy data to a
	// staging buffer (possibly creating that first if necessary), then copying
	// from that staging buffer to CPU
	D3D_FEATURE_LEVEL level = mDevice->GetFeatureLevel();
	if (level < D3D_FEATURE_LEVEL_11_0 && flags == MAP_READONLY) {
		if (!mD3D11StagingBuffer) {
			int buflen = mCount * mSize;

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
	}
	else {
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
				return mSR.pData;
			}
		}
	}

	return nullptr;
}

void IndexBuffer::unmap()
{
	assert(mSR.pData);

	if (mDevice && mSR.pData) {
		ComPtr<ID3D11DeviceContext> ctx;
		mDevice->GetImmediateContext(&ctx);

		ctx->Unmap(mD3D11Buffer.Get(), 0);

		ZeroMemory(&mSR, sizeof(mSR));
	}
}

ID3D11Buffer* IndexBuffer::buffer()
{
	return mD3D11Buffer.Get();
}

} // Direct3D11
} // Gfx
} // Teardrop
