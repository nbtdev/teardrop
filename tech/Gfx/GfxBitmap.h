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

#if !defined(COSGFXBITMAP_INCLUDED)
#define COSGFXBITMAP_INCLUDED

#include "Gfx/GfxCommon.h"

namespace CoS
{
	class Stream;

	class GfxBitmap
	{
	public:
		GfxBitmap();
		~GfxBitmap();

		typedef enum
		{
			UNKNOWN,
			RAW,
			BMP,
			TGA,
			DXT1,
			DXT2,
			DXT3,
			DXT4,
			DXT5,
		} Format;

		//! load bitmap image from arbitrary stream
		bool load(Stream& stream);
		//! load bitmap image from disk
		bool load(const char* pFilename);
		//! load bitmap image from in-memory data
		bool load(void* pData, Format fmt=RAW, 
			size_t sx=0, size_t sy=0, size_t depth=0);

		/** Accessors
		*/
		size_t getWidth() const { return m_width; }
		size_t getHeight() const { return m_height; }
		size_t getDepth() const { return m_depth; }
		size_t getMips() const { return m_mips; }
		void* getData() const { return m_pData; }
		Format getFormat() const { return m_fmt; }

		DECLARE_GFX_ALLOCATOR();

	private:
		void* m_pData;
		size_t m_width;
		size_t m_height;
		size_t m_depth;
		size_t m_mips;
		Format m_fmt;

		bool loadBMP(const char* pFilename);
		bool loadTGA(const char* pFilename);
		bool loadRawData(const char* pFilename);
		bool loadRawData(Stream& stream);
	};
}

#endif // COSGFXBITMAP_INCLUDED
