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

#if !defined(GFXTEXTURED3D9_INCLUDED)
#define GFXTEXTURED3D9_INCLUDED

#include "GfxTexture.h"

namespace CoS
{
	struct Environment;

	class GfxTextureD3D9 : public GfxTexture
	{
		DECLARE_SERIALIZABLE(GfxTextureD3D9);

		SerialPointer<IDirect3DTexture9> m_pTex;

	public:
		GfxTextureD3D9();
		GfxTextureD3D9(int);
		~GfxTextureD3D9();

		bool initialize(
			unsigned int width, 
			unsigned int height, 
			unsigned int depth, 
			Format format, 
			Type type, 
			size_t bDynamic,
			unsigned int mips=0, 
			void* pData=0,
			unsigned int dataLen=0);

		bool destroy();
		bool release();
		void releaseD3DResources();
		bool delayInit();
		void* lock(int& pitch, bool bDiscardExisting=true);
		void unlock();
		IDirect3DTexture9* getD3D9Texture();
		bool setToSampler(size_t sampler);

		bool serialize(ResourceSerializer& serializer);

		DECLARE_GFX_ALLOCATOR();

	private:
		bool initD3D9Tex(size_t bDynamic);
	};
}

#endif // GFXTEXTURED3D9_INCLUDED
