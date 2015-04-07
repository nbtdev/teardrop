/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INDEXBUFFERD3D11_INCLUDED)
#define INDEXBUFFERD3D11_INCLUDED

#include "Gfx/IndexBuffer.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class IndexBuffer : public Gfx::IndexBuffer
			{
			public:
				IndexBuffer(ComPtr<ID3D11Device> aDevice, Submesh* parent);
				~IndexBuffer();

				void initialize(int indexCount, int aInitFlags, void* data = 0);
				void resize(int indexCount);
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

#endif // INDEXBUFFERD3D11_INCLUDED
