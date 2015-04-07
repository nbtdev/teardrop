/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
				ComPtr<ID3D11Buffer> mD3D11Buffer;

				// for D3D10 devices, we can only read from staging buffers
				ComPtr<ID3D11Buffer> mD3D11StagingBuffer;

				ComPtr<ID3D11Device> mDevice;
				D3D11_MAPPED_SUBRESOURCE mSR;

				bool mMappedStaging = false;
			};
		}
	}
}

#endif // VERTEXBUFFERD3D11_INCLUDED
