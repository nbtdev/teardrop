/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERABLE_INCLUDED)
#define RENDERABLE_INCLUDED

#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class IndexBuffer;
		class VertexBuffer;

		struct Renderable
		{
		public:
			Renderable();

			enum PrimitiveType
			{
				PT_POINT = 0,
				PT_LINES,
				PT_LINE_STRIP,
				PT_TRIANGLES,
				PT_TRIANGLE_STRIP,
				PT_MAX
			};

			IndexBuffer* mIndexBuffer;
			int mElementOffset;
			VertexBuffer** mVertexBuffers;
			int mNumVertexBuffers;
			PrimitiveType mPrimType;
		};
	}
}

#endif // RENDERABLE_INCLUDED
