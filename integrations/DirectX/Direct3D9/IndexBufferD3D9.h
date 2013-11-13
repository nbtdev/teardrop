/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INDEXBUFFERD3D9_INCLUDED)
#define INDEXBUFFERD3D9_INCLUDED

#include "Gfx/IndexBuffer.h"

namespace Teardrop
{
	namespace Gfx 
	{
		namespace Direct3D9
		{
			class IndexBuffer : public Gfx::IndexBuffer
			{
			public:
				IndexBuffer(IDirect3DDevice9* device, Submesh* parent);
				~IndexBuffer();

				bool initialize(int indexSize, int indexCount, void* data=0);
				void resize(int indexSize, int indexCount);
				void* map(int flags=0);
				void unmap();

				IDirect3DIndexBuffer9* buffer();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DIndexBuffer9* mD3D9Buffer;
				IDirect3DDevice9* mDevice;
			};
		}
	}
}

#endif // INDEXBUFFERD3D9_INCLUDED
