/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
			void removeVertexBuffer(int index);

			int vertexBufferCount();
			VertexBuffer* vertexBuffer(int index);
			IndexBuffer* indexBuffer();

			VertexDeclaration* vertexDeclaration();
			void clearVertexDeclaration();
			
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
