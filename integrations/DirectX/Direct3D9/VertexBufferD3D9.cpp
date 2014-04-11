/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VertexBufferD3D9.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

VertexBuffer::VertexBuffer(IDirect3DDevice9* device, Submesh* parent)
	: Gfx::VertexBuffer(parent)
	, mD3D9Buffer(0)
	, mDevice(device)
	, mD3D9Desc(0)
	, mInitFlags(0)
{
}

VertexBuffer::~VertexBuffer()
{
	if (mD3D9Buffer)
	{
		mD3D9Buffer->Release();
	}

	delete [] mD3D9Desc;
}

bool VertexBuffer::initialize(int vertexCount, int initFlags, void* data/* =0 */)
{
	// create D3D9 vertex buffer in the default pool
	size_t bufSize = vertexSize() * vertexCount;
	if (!bufSize)
		return false;

	DWORD f = 0;
	if (initFlags & INIT_DYNAMIC)
		f |= D3DUSAGE_DYNAMIC;
	if (initFlags & INIT_STATIC)
		f |= D3DUSAGE_WRITEONLY;
	if (initFlags & INIT_WRITEONLY)
		f |= D3DUSAGE_WRITEONLY;

	HRESULT hr = mDevice->CreateVertexBuffer(
		(UINT)bufSize,
		f,
		0, 
		D3DPOOL_DEFAULT,
		&mD3D9Buffer,
		0);

	if (FAILED(hr))
	{
		//env.pLogger->logMessage("Could not create vertex buffer...");
		//pRend->logD3DError(hr);
		return false;
	}

	// copy our data into the D3D9 buffer
	if (data) {
		void* ppbData;
		hr = mD3D9Buffer->Lock(
			0, 
			(UINT)bufSize, 
			&ppbData, 
			0);

		if (FAILED(hr))
		{
			//env.pLogger->logMessage("Could not lock vertex buffer...");
			//pRend->logD3DError(hr);
			return false;
		}

		memcpy(ppbData, data, bufSize);
		mD3D9Buffer->Unlock();
	}

	mInitFlags = initFlags;
	mCount = vertexCount;
	return true;
}

void VertexBuffer::resize(int vertexCount)
{
	if (mD3D9Buffer)
		mD3D9Buffer->Release();

	initialize(vertexCount, mInitFlags);
}

void* VertexBuffer::map(int flags /*=0*/)
{
	DWORD D3DFlags = 0;
	if ((mInitFlags & INIT_DYNAMIC) && (flags & MAP_DISCARD))
		D3DFlags |= D3DLOCK_DISCARD;
	if (flags & MAP_READONLY)
		D3DFlags |= D3DLOCK_READONLY;

	if (mD3D9Buffer) {
		void* rtn = 0;
		mD3D9Buffer->Lock(0, 0, &rtn, D3DFlags);
		return rtn;
	}

	return 0;
}

void VertexBuffer::unmap()
{
	if (mD3D9Buffer) {
		mD3D9Buffer->Unlock();
	}
}

IDirect3DVertexBuffer9* VertexBuffer::buffer()
{
	return mD3D9Buffer;
}

} // Direct3D9
} // Gfx
} // Teardrop
