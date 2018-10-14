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

#if !defined(BUFFERMANAGERD3D11_INCLUDED)
#define BUFFERMANAGERD3D11_INCLUDED

#include "stdafx.h"
#include "Gfx/BufferManager.h"

#include <list>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class Renderer;
			class IndexBuffer;
			class VertexBuffer;
			class VertexDeclaration;

			class BufferManager : public Gfx::BufferManager
			{
			public:
				BufferManager(Renderer* aRenderer);
				~BufferManager();

				Gfx::IndexBuffer* createIndexBuffer(Submesh* parent);
				Gfx::VertexBuffer* createVertexBuffer(Submesh* parent);
				Gfx::VertexDeclaration* createVertexDeclaration(Submesh* parent);
				void release(Gfx::IndexBuffer* buffer);
				void release(Gfx::VertexBuffer* buffer);
				void release(Gfx::VertexDeclaration* decl);

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;

				typedef std::list<IndexBuffer*> IndexBuffers;
				typedef std::list<VertexBuffer*> VertexBuffers;
				typedef std::list<VertexDeclaration*> VertexDeclarations;
				IndexBuffers mIndexBuffers;
				VertexBuffers mVertexBuffers;
				VertexDeclarations mVertexDeclarations;
			};
		}
	}
}

#endif // BUFFERMANAGERD3D11_INCLUDED
