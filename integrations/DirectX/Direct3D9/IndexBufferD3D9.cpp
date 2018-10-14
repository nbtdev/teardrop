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
#include "IndexBufferD3D9.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

IndexBuffer::IndexBuffer(IDirect3DDevice9* device, Submesh* parent)
	: Gfx::IndexBuffer(parent)
	, mD3D9Buffer(0)
	, mDevice(device)
{
	assert(mDevice);
	if (mDevice)
		mDevice->AddRef();
}

IndexBuffer::~IndexBuffer()
{
	if (mD3D9Buffer) {
		mD3D9Buffer->Release();
	}

	if (mDevice)
		mDevice->Release();
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

void* IndexBuffer::map(MapFlags flags /*=MAP_ANY*/)
{
	DWORD D3DFlags = 0;
	// TODO: if we allow dynamic flags on create then we can lock discard
	//if (flags & MAP_DISCARD)
	//	D3DFlags |= D3DLOCK_DISCARD;
	// TODO: D3D gets upset if you try to lock a write-only buffer as read-only...
	//if (flags & MAP_READONLY)
	//	D3DFlags |= D3DLOCK_READONLY;

	if (mD3D9Buffer) {
		void* rtn = 0;
		mD3D9Buffer->Lock(0, 0, &rtn, D3DFlags);
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
