/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "BufferManagerOpenGL.h"
#include "IndexBufferOpenGL.h"
#include "VertexBufferOpenGL.h"
#include "VertexDeclarationOpenGL.h"

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

BufferManager::BufferManager()
{
}

BufferManager::~BufferManager()
{
	for (auto b : mIndexBuffers) {
		delete b;
	}

	for (auto b : mVertexBuffers) {
		delete b;
	}

	for (auto v : mVertexDeclarations) {
		delete v;
	}
}

Gfx::IndexBuffer* BufferManager::createIndexBuffer(Submesh* parent)
{
	IndexBuffer* buf = TD_NEW IndexBuffer(parent);
	mIndexBuffers.push_back(buf);
    return buf;
}

Gfx::VertexBuffer* BufferManager::createVertexBuffer(Submesh* parent)
{
    VertexBuffer* buf = TD_NEW VertexBuffer(parent);
	mVertexBuffers.push_back(buf);
    return buf;
}

Gfx::VertexDeclaration* BufferManager::createVertexDeclaration(Submesh* parent)
{
    VertexDeclaration* buf = TD_NEW VertexDeclaration(parent);
	mVertexDeclarations.push_back(buf);
    return buf;
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
