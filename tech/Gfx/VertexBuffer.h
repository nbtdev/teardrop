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

#if !defined(VERTEXBUFFER_INCLUDED)
#define VERTEXBUFFER_INCLUDED

#include "Gfx/VertexElement.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class Submesh;

		class VertexBuffer
		{
		public:
			VertexBuffer(Submesh* parent);
			virtual ~VertexBuffer();

			int vertexCount();
			int vertexSize();

			void beginAddVertexElements();
			VertexElement& addVertexElement();
			int endAddVertexElements();
			int vertexElementCount();
			VertexElement* vertexElement(int index);
			
			enum InitFlags {
				INIT_DYNAMIC = 1,
				INIT_STATIC = 2,
				INIT_WRITEONLY = 4,
				INIT_READWRITE = 8,
			};
			
			enum MapFlags {
				MAP_ANY = 0,
				MAP_READONLY = 1,
				MAP_DISCARD = 2,
			};

			virtual void initialize(int vertexCount, int initFlags, void* data=0) = 0;
			virtual void resize(int vertexCount) = 0;
			virtual void* map(MapFlags mapFlags) = 0;
			virtual void unmap() = 0;

			TD_DECLARE_ALLOCATOR();

		protected:
			int mSize;
			int mCount;
			int mInitFlags;

			typedef std::vector<VertexElement> VertexElements;
			VertexElements mElements;

			Submesh* mParent;
		};
	}
}

#endif // VERTEXBUFFER_INCLUDED
