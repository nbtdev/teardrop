/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "integration/Direct3D9/BufferManagerD3D9.h"
#include "integration/Direct3D9/IndexBufferD3D9.h"
#include "integration/Direct3D9/VertexBufferD3D9.h"

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

BufferManager::BufferManager(IDirect3DDevice9* device)
	: mDevice(device)
{
}

BufferManager::~BufferManager()
{
	for (IndexBuffers::iterator it = mIndexBuffers.begin(); it != mIndexBuffers.end(); ++it) {
		delete *it;
	}

	for (VertexBuffers::iterator it = mVertexBuffers.begin(); it != mVertexBuffers.end(); ++it) {
		delete *it;
	}
}

Gfx::IndexBuffer* BufferManager::createIndexBuffer()
{
	Gfx::IndexBuffer* rtn = TD_NEW IndexBuffer(mDevice);
	return rtn;
}

Gfx::VertexBuffer* BufferManager::createVertexBuffer()
{
	Gfx::VertexBuffer* rtn = TD_NEW VertexBuffer(mDevice);
	return rtn;
}

void BufferManager::release(Gfx::IndexBuffer* buffer)
{
	// TODO: refcounting?
	mIndexBuffers.remove(static_cast<IndexBuffer*>(buffer));
	delete buffer;
}

void BufferManager::release(Gfx::VertexBuffer* buffer)
{
	// TODO: refcounting?
	mVertexBuffers.remove(static_cast<VertexBuffer*>(buffer));
	delete buffer;
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
