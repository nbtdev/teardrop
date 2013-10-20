/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BUFFERMANAGERD3D9_INCLUDED)
#define BUFFERMANAGERD3D9_INCLUDED

#include "Gfx/BufferManager.h"
#include <list>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D9
		{
			class IndexBuffer;
			class VertexBuffer;

			class BufferManager : public Gfx::BufferManager
			{
			public:
				BufferManager(IDirect3DDevice9* device);
				~BufferManager();

				Gfx::IndexBuffer* createIndexBuffer();
				Gfx::VertexBuffer* createVertexBuffer();
				void release(Gfx::IndexBuffer* buffer);
				void release(Gfx::VertexBuffer* buffer);

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;

				typedef std::list<IndexBuffer*> IndexBuffers;
				typedef std::list<VertexBuffer*> VertexBuffers;
				IndexBuffers mIndexBuffers;
				VertexBuffers mVertexBuffers;
			};
		}
	}
}

#endif // BUFFERMANAGERD3D9_INCLUDED
