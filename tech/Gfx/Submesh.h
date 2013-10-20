/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SUBMESH_INCLUDED)
#define SUBMESH_INCLUDED

#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class IndexBuffer;
		class VertexBuffer;

		class Submesh
		{
		public:
			Submesh();
			~Submesh();

			IndexBuffer* createIndexBuffer();
			VertexBuffer* createVertexBuffer();
			void removeIndexBuffer();
			void removeVertexBuffer(int index);

			int vertexBufferCount();
			VertexBuffer* vertexBuffer(int index);
			IndexBuffer* indexBuffer();

			TD_DECLARE_ALLOCATOR();

		protected:
			typedef std::vector<VertexBuffer*> VertexBuffers;
			VertexBuffers mVertexBuffers;
			IndexBuffer* mIndexBuffer;
		};
	}
}

#endif // SUBMESH_INCLUDED
