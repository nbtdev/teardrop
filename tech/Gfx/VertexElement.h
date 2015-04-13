/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(VERTEXELEMENT_INCLUDED)
#define VERTEXELEMENT_INCLUDED

#include "Memory/Allocators.h"

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
