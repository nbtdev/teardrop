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

#if !defined(VERTEXBUFFERD3D11_INCLUDED)
#define VERTEXBUFFERD3D11_INCLUDED

#include "Gfx/VertexBuffer.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class VertexBuffer : public Gfx::VertexBuffer
			{
			public:
				VertexBuffer(ComPtr<ID3D11Device> aDevice, Submesh* parent);
				~VertexBuffer();

				void initialize(int aVertexCount, int aInitFlags, void* data = 0);
				void resize(int aVertexCount);
				void* map(MapFlags flags = MAP_ANY);
				void unmap();

				ID3D11Buffer* buffer();

				TD_DECLARE_ALLOCATOR();

			protected:
				// for D3D10 devices, we can only read from staging buffers
				ComPtr<ID3D11Buffer> mD3D11StagingBuffer;

				ComPtr<ID3D11Buffer> mD3D11Buffer;
				ComPtr<ID3D11Device> mDevice;
				D3D11_MAPPED_SUBRESOURCE mSR;
				MapFlags mMapFlags = MAP_ANY;
			};
		}
	}
}

#endif // VERTEXBUFFERD3D11_INCLUDED
