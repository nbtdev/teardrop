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

void IndexBuffer::initialize(int indexSize, int indexCount, void* data/* =0 */)
{
	CD3D11_BUFFER_DESC desc(
		indexSize * indexCount,
		D3D11_BIND_INDEX_BUFFER,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE
		);

	D3D11_SUBRESOURCE_DATA sd = { 0 };
	D3D11_SUBRESOURCE_DATA* psd = nullptr;

	if (data) {
		sd.pSysMem = data;
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
	}
}

void IndexBuffer::resize(int indexSize, int indexCount)
{
	if (mD3D11Buffer)
		mD3D11Buffer.Reset();

	initialize(indexSize, indexCount);
}

void* IndexBuffer::map(MapFlags flags /*=MAP_ANY*/)
{
	assert(!mSR.pData);
	if (mSR.pData) {
		throw BufferMappedException("Index buffer already mapped", mSR.pData);
	}

	D3D11_MAP D3DFlags;
	
	// TODO: if we allow dynamic flags on create then we can lock discard
	
	if (flags & MAP_DISCARD)
		D3DFlags = D3D11_MAP_WRITE_DISCARD;
	
	// TODO: D3D gets upset if you try to lock a write-only buffer as read-only...
	
	if (flags & MAP_READONLY)
		D3DFlags = D3D11_MAP_READ;

	if (mD3D11Buffer && mDevice) {
		void* rtn = 0;
		ComPtr<ID3D11DeviceContext> ctx;
		mDevice->GetImmediateContext(&ctx);

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
