/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INDEXBUFFER_INCLUDED)
#define INDEXBUFFER_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Submesh;

		class IndexBuffer
		{
		public:
			IndexBuffer(Submesh* parent);
			virtual ~IndexBuffer();

			int indexCount();
			int triangleCount();
			int indexSize();

			enum MapFlags {
                MAP_ANY = 0,
				MAP_READONLY = 1,
				MAP_DISCARD = 2,
                MAP_WRITEONLY = 3
			};

			virtual bool initialize(int indexSize, int indexCount, void* data=0) = 0;
			virtual void resize(int indexSize, int indexCount) = 0;
            virtual void* map(MapFlags flags=MAP_ANY) = 0;
			virtual void unmap() = 0;

			TD_DECLARE_ALLOCATOR();

		protected:
			int mSize;
			int mCount;
			Submesh* mParent;
		};
	}
}

#endif // INDEXBUFFER_INCLUDED
