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

#if !defined(SUBMESH_INCLUDED)
#define SUBMESH_INCLUDED

#include "Memory/Allocators.h"
#include "Gfx/ShaderFeatures.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class IndexBuffer;
		class VertexBuffer;
		class VertexDeclaration;

		class Submesh
		{
		public:
			Submesh();
			~Submesh();

			IndexBuffer* createIndexBuffer();
			VertexBuffer* createVertexBuffer();
			void removeIndexBuffer();
            void removeVertexBuffer(size_t index);
            void clearVertexDeclaration();
            VertexDeclaration* vertexDeclaration();

            size_t vertexBufferCount() const;
            VertexBuffer* vertexBuffer(size_t index) const;
            IndexBuffer* indexBuffer() const;
			
			enum PrimitiveType
			{
				PT_UNKNOWN = 0,
				PT_POINTLIST,
				PT_LINELIST,
				PT_LINESTRIP,
				PT_TRILIST,
				PT_TRISTRIP,
				PT_TRIFAN,
			};

			void setPrimitiveType(PrimitiveType type);
			PrimitiveType primitiveType();

			const ShaderFeatures& features();
			void clearFeatures();

			unsigned int hash();

			TD_DECLARE_ALLOCATOR();

		protected:
			ShaderFeatures mFeatures;
			typedef std::vector<VertexBuffer*> VertexBuffers;
			VertexBuffers mVertexBuffers;
			VertexDeclaration* mVertexDeclaration;
			IndexBuffer* mIndexBuffer;
			PrimitiveType mPrimitiveType;
			unsigned int mHash;
		};
	}
}

#endif // SUBMESH_INCLUDED
