/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "integration/Direct3D9/IndexBufferD3D9.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

IndexBuffer::IndexBuffer(IDirect3DDevice9* device, Submesh* parent)
	: Gfx::IndexBuffer(parent)
	, mD3D9Buffer(0)
	, mDevice(device)
{
}

IndexBuffer::~IndexBuffer()
{
	if (mD3D9Buffer)
	{
		mD3D9Buffer->Release();
	}
}

bool IndexBuffer::initialize(int indexSize, int indexCount, void* data/* =0 */)
{
	HRESULT hr = mDevice->CreateIndexBuffer(
		(UINT)indexCount * indexSize,
		D3DUSAGE_WRITEONLY,
		indexSize == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&mD3D9Buffer,
		0);

	if (FAILED(hr)) {
	//	env.pLogger->logMessage("Could not create index buffer...");
	//	pRend->logD3DError(hr);
	//	return false;
	}
	else {
		mSize = indexSize;
		mCount = indexCount;
	}

	if (data) {
		// copy the index data to our new D3D9 buffer
		void* ppbData;
		hr = mD3D9Buffer->Lock(
			0, 
			(UINT)indexSize * indexCount,
			&ppbData,
			0);

		//if (FAILED(hr))
		//{
		//	env.pLogger->logMessage("Could not lock index buffer...");
		//	pRend->logD3DError(hr);
		//	return false;
		//}

		memcpy(ppbData, data, (UINT)indexSize * indexCount);
		mD3D9Buffer->Unlock();
	}

	return true;
}

void IndexBuffer::resize(int indexSize, int indexCount)
{
	if (mD3D9Buffer)
		mD3D9Buffer->Release();

	initialize(indexSize, indexCount);
}

void* IndexBuffer::map(int flags /*=0*/)
{
	DWORD D3DFlags = 0;
	if (flags & MAP_DISCARD)
		D3DFlags |= D3DLOCK_DISCARD;
	if (flags & MAP_READONLY)
		D3DFlags |= D3DLOCK_READONLY;

	if (mD3D9Buffer) {
		void* rtn = 0;
		mD3D9Buffer->Lock(0, 0, &rtn, flags);
		return rtn;
	}

	return 0;
}

void IndexBuffer::unmap()
{
	if (mD3D9Buffer) {
		mD3D9Buffer->Unlock();
	}
}

IDirect3DIndexBuffer9* IndexBuffer::buffer()
{
	return mD3D9Buffer;
}

} // Direct3D9
} // Gfx
} // Teardrop
