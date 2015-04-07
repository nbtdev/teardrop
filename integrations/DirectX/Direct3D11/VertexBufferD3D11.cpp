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
	UINT cpuFlags = 0;

	if (aInitFlags & INIT_DYNAMIC) {
		assert((aInitFlags & INIT_WRITEONLY) && "If you specify INIT_DYNAMIC on a D3D10/11 buffer, you must also specify INIT_WRITEONLY");
		usage = D3D11_USAGE_DYNAMIC;

		if (aInitFlags & INIT_WRITEONLY)
			cpuFlags = D3D11_CPU_ACCESS_WRITE;
	}

	int buflen = aVertexCount * vertexSize();

	CD3D11_BUFFER_DESC desc(
		buflen,
		D3D11_BIND_VERTEX_BUFFER,
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

	if (!(mInitFlags & INIT_DYNAMIC)) {
		// first, create the staging buffer if it does not already exist
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

		// then, if the map type is MAP_READONLY, copy the original resource to the staging buffer...
		if (flags == MAP_READONLY)
			ctx->CopyResource(mD3D11StagingBuffer.Get(), mD3D11Buffer.Get());

		// regardless, map the staging buffer and return that
		HRESULT hr = ctx->Map(
			mD3D11StagingBuffer.Get(),
			0,
			D3D11_MAP_READ,
			0,
			&mSR);

		mMapFlags = flags;

		if (SUCCEEDED(hr)) {
			return mSR.pData;
		}
	}
	else {
		// otherwise, if the initialization flags include INIT_DYNAMIC, then do normal map (but
		// only if they also include INIT_WRITEONLY -- we should already have verified this
		// earlier). Therefore, we only support WRITE mapping here (and we will do WRITE_DISCARD
		// for efficiency, until other needs dictate)

		if (mD3D11Buffer && mDevice) {
			HRESULT hr = ctx->Map(
				mD3D11Buffer.Get(),
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&mSR);

			if (SUCCEEDED(hr)) {
				return mSR.pData;
			}
		}
	}

	return nullptr;
}

void VertexBuffer::unmap()
{
	assert(mSR.pData);
	assert(mMapFlags != MAP_ANY);

	if (mDevice && mSR.pData) {
		ComPtr<ID3D11DeviceContext> ctx;
		mDevice->GetImmediateContext(&ctx);

		// when we unmap, if the map operation was a write to a staging buffer (which 
		// would be the case for all buffers not created with INIT_DYNAMIC), then we need
		// to copy the staging buffer over to the live one after unmapping the staging buffer.
		// We don't need to check anything at this point -- if they got here, then the flag 
		// combinations have all been verified and we know that we have an open map to unmap.

		if (mInitFlags & INIT_DYNAMIC) {
			// in this case just do a normal unmap
			ctx->Unmap(mD3D11Buffer.Get(), 0);
		}
		else {
			// do the complex unmap-and-copy; in any case, we need to unmap the staging buffer
			ctx->Unmap(mD3D11StagingBuffer.Get(), 0);

			if (mMapFlags != MAP_READONLY) {
				// then it was a write and needs to be "flushed"; there should be no other 
				// possible combination at this point than this
				ctx->CopyResource(mD3D11Buffer.Get(), mD3D11StagingBuffer.Get());
			}
		}

		mMapFlags = MAP_ANY;
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
