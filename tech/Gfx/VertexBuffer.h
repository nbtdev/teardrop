/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXBUFFER_INCLUDED)
#define VERTEXBUFFER_INCLUDED

#include "Gfx/VertexElement.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class VertexBuffer
		{
		public:
			VertexBuffer();
			virtual ~VertexBuffer();

			int vertexCount();
			int vertexSize();

			void beginAddVertexElements();
			VertexElement& addVertexElement();
			int endAddVertexElements();
			int elementCount();
			
			enum InitFlags {
				INIT_DYNAMIC = 1,
				INIT_STATIC = 2,
				INIT_WRITEONLY = 4,
			};
			
			enum MapFlags {
				MAP_READONLY = 1,
				MAP_DISCARD = 2,
			};

			virtual bool initialize(int vertexCount, int initFlags, void* data=0) = 0;
			virtual void resize(int vertexCount) = 0;
			virtual void* map(int mapFlags) = 0;
			virtual void unmap() = 0;

			TD_DECLARE_ALLOCATOR();

		protected:
			int mSize;
			int mCount;

			typedef std::vector<VertexElement> VertexElements;
			VertexElements mElements;
		};
	}
}

#endif // VERTEXBUFFER_INCLUDED
