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

#if !defined(VERTEXBUFFERD3D9_INCLUDED)
#define VERTEXBUFFERD3D9_INCLUDED

#include "Gfx/VertexBuffer.h"

namespace Teardrop
{
	namespace Gfx 
	{
		namespace Direct3D9
		{
			class VertexBuffer : public Gfx::VertexBuffer
			{
			public:
				VertexBuffer(IDirect3DDevice9* device, Submesh* parent);
				~VertexBuffer();

				bool initialize(int vertexCount, int initFlags, void* data=0);
				void resize(int vertexCount);
				void* map(MapFlags mapFlags);
				void unmap();

				IDirect3DVertexBuffer9* buffer();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DVertexBuffer9* mD3D9Buffer;
				IDirect3DDevice9* mDevice;
				D3DVERTEXELEMENT9* mD3D9Desc;
				int mInitFlags;
			};
		}
	}
}

#endif // VERTEXBUFFERD3D9_INCLUDED
