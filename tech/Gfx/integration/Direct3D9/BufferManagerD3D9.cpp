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
#include "integration/Direct3D9/VertexDeclarationD3D9.h"

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

Gfx::IndexBuffer* BufferManager::createIndexBuffer(Submesh* parent)
{
	Gfx::IndexBuffer* rtn = TD_NEW IndexBuffer(mDevice, parent);
	return rtn;
}

Gfx::VertexBuffer* BufferManager::createVertexBuffer(Submesh* parent)
{
	Gfx::VertexBuffer* rtn = TD_NEW VertexBuffer(mDevice, parent);
	return rtn;
}

Gfx::VertexDeclaration* BufferManager::createVertexDeclaration(Submesh* parent)
{
	Gfx::VertexDeclaration* rtn = TD_NEW VertexDeclaration(mDevice, parent);
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

void BufferManager::release(Gfx::VertexDeclaration* decl)
{
	// TODO: refcounting?
	mVertexDeclarations.remove(static_cast<VertexDeclaration*>(decl));
	delete decl;
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
