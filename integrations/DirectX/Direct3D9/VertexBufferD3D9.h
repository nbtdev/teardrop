/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
