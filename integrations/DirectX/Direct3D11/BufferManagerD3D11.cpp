/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "BufferManagerD3D11.h"
#include "IndexBufferD3D11.h"
#include "RendererD3D11.h"
#include "VertexBufferD3D11.h"
#include "VertexDeclarationD3D11.h"

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

BufferManager::BufferManager(Renderer* aRenderer)
	: mDevice(aRenderer->device())
{
}

BufferManager::~BufferManager()
{
	for (auto ib : mIndexBuffers) {
		delete ib;
	}

	for (auto vb : mVertexBuffers) {
		delete vb;
	}

	for (auto decl : mVertexDeclarations) {
		delete decl;
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

} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
