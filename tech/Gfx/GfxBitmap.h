/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXBITMAP_INCLUDED)
#define TDGFXBITMAP_INCLUDED

#include "Gfx/GfxCommon.h"

namespace Teardrop
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

		bool loadTGA(const char* pFilename);
		bool loadRawData(const char* pFilename);
		bool loadRawData(Stream& stream);
	};
}

#endif // TDGFXBITMAP_INCLUDED
