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

void VertexBuffer::initialize(int aVertexCount, int aInitFlags, void* data/* =0 */)
{
	CD3D11_BUFFER_DESC desc(
		aVertexCount * vertexSize(),
		D3D11_BIND_VERTEX_BUFFER,
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
		throw Exception("Could not create D3D11 vertex buffer in VertexBuffer::initialize");
	}
	else {
		mInitFlags = aInitFlags;
		mCount = aVertexCount;
	}
}

void VertexBuffer::resize(int aVertexCount)
{
	if (mD3D11Buffer)
		mD3D11Buffer.Reset();

	initialize(aVertexCount, mInitFlags);
}

void* VertexBuffer::map(MapFlags flags /*=MAP_ANY*/)
{
	assert(!mSR.pData);
	if (mSR.pData) {
		throw BufferMappedException("Vertex buffer already mapped", mSR.pData);
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

void VertexBuffer::unmap()
{
	assert(mSR.pData);

	if (mDevice && mSR.pData) {
		ComPtr<ID3D11DeviceContext> ctx;
		mDevice->GetImmediateContext(&ctx);

		ctx->Unmap(mD3D11Buffer.Get(), 0);

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
