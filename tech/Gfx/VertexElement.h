/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXELEMENT_INCLUDED)
#define VERTEXELEMENT_INCLUDED

namespace Teardrop
{
	namespace Gfx 
	{
		enum VertexElementType {
			VET_UNKNOWN,
			VET_BYTE,
			VET_SHORT,
			VET_HALF,
			VET_FLOAT,
		};

		enum VertexElementUsage {
			VEU_UNKNOWN,
			VEU_POSITION,
			VEU_NORMAL,
			VEU_BLENDWEIGHT,
			VEU_BLENDINDEX,
			VEU_TEXCOORD,
			VEU_TANGENT,
			VEU_BINORMAL,
			VEU_COLOR,
		};

		struct VertexElement
		{
			VertexElement();

			// vertex element size in bytes
			int size();

			// element data math type
			VertexElementType mType;

			// number of elements (usually [1..4])
			int mCount;

			// what this element is meant to be used for 
			VertexElementUsage mUsage;

			// the zero-based Nth instance of this element in the vertex (i.e. 2 for TEXCOORD2)
			int mIndex;

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // VERTEXELEMENT_INCLUDED
