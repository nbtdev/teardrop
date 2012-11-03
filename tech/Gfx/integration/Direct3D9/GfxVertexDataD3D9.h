/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(GFXVERTEXDATAD3D9_INCLUDED)
#define GFXVERTEXDATAD3D9_INCLUDED

#include "GfxVertexData.h"

namespace CoS
{
	struct Environment; 

	class GfxVertexDataD3D9 : public GfxVertexData
	{
		DECLARE_SERIALIZABLE(GfxVertexDataD3D9);

		SerialPointer<IDirect3DVertexBuffer9> m_pD3D9VertexBuffer;

	public:
		GfxVertexDataD3D9();
		GfxVertexDataD3D9(int);
		~GfxVertexDataD3D9();

		bool initialize(
			CreationFlags flags=STATIC, 
			unsigned int vertSize=0, 
			unsigned int vertCount=0, 
			void* pData=0);

		bool destroy();
		bool release();
		void releaseD3DResources();
		void* lock(bool bDiscardExisting=true);
		void unlock();
		IDirect3DVertexBuffer9* getD3D9VertexBuffer();
		bool delayInit();

		bool serialize(ResourceSerializer& serializer);

		DECLARE_GFX_ALLOCATOR();

	public:
		bool initD3D9();
	};
}

#endif // GFXVERTEXDATAD3D9_INCLUDED
