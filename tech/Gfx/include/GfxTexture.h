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

#if !defined(COSGFXTEXTURE_INCLUDED)
#define COSGFXTEXTURE_INCLUDED

#include "Resource/include/Resource.h"

namespace CoS
{
	class ResourceSerializer;
	class Stream;
	struct FourCC;

	class GfxTexture : public Resource
	{
		DECLARE_SERIALIZABLE(GfxTexture);
		DECLARE_SERIALIZABLE_VTABLE

	public:
		static const FourCC& RESOURCE_TYPE;

		//! normal c'tor (cannot fail)
		GfxTexture();
		//! placement c'tor (cannot fail)
		GfxTexture(int);
		//! d'tor (cannot fail)
		virtual ~GfxTexture();

		enum Format
		{
			UNKNOWN=0,
			TGA,
			DXT1,
			DXT2,
			DXT3,
			DXT4,
			DXT5,

			// typically for render targets
			TEXF_FLOAT32,
			TEXF_FLOAT64,
			TEXF_FLOAT128,

			FORMAT_FORCE_DWORD = 0xFFFFFFFF,
		};

		enum Type
		{
			TEXUNKNOWN			= 0x00000000,
			TEX1D				= 0x00010000,
			TEX2D				= 0x00020000,
			TEX3D				= 0x00040000,
			TEXVOLUME			= 0x00080000,
			TEXCUBE				= 0x00100000,

			// bitwise-or this into applicable types (such as TEX2D)
			TEX_RENDERTARGET	= 0x01000000,
			TEX_DYNAMIC			= 0x02000000,
		};

		//! create and initialize the texture (anything that can fail)
		virtual bool initialize(
			unsigned int width, 
			unsigned int height, 
			unsigned int depth, 
			Format format, 
			Type type, 
			size_t bDynamic,
			unsigned int mips=0, 
			void* pData=0,
			unsigned int dataLen=0);
		//! delete the texture
		virtual bool destroy();
		//! release dynamic resource data
		virtual bool release();
		//! lock the texture data for editing
		virtual void* lock(int& pitch, bool bDiscardExisting=true);
		//! unlock the texture data when done editing
		virtual void unlock();
		
		/** accessors
		*/
		//! return texture width
		unsigned int getWidth() const;
		//! return texture height
		unsigned int getHeight() const;
		//! return texture depth
		unsigned int getDepth() const;
		//! return texture format
		Format getFormat() const;
		//! return number of mip maps present in texture
		unsigned int getNumMips() const;
		//! return texture filename (if available/applicable)
		const char* getName() const;
		//! get texture type 
		Type getType() const;
		//! return texture depth
		unsigned int getDataSize() const;

		/** mutators
		*/
		bool setTextureData(
			unsigned int width, 
			unsigned int height, 
			unsigned int depth, 
			Format format, 
			Type type, 
			unsigned int mips=0, 
			void* pData=0,
			unsigned int dataLen=0);

		//! set texture filename (if available/applicable)
		void setName(const char* pName);
		//! loads texture data from stream; owns the data once loaded
		bool loadTextureData(Stream& stream);

		/**
			Serialization
		*/
		//! package for storage
		virtual bool serialize(ResourceSerializer& serializer);

		// for use by renderer implementation
		virtual bool setToSampler(size_t sampler);

		DECLARE_GFX_ALIGNED_ALLOCATOR();

	protected:
		//! width in texels
		unsigned int m_width;
		//! height in texels
		unsigned int m_height;
		//! depth in bytes per pixel (texels for volume textures)
		unsigned int m_depth;
		//! texture format (DXTn, TGA, FP, etc)
		Format m_format;
		//! number of mipmaps present in texture (1 if only first mip is present)
		unsigned int m_numMips;
		//! format-dependent flag data
		Type m_type;
		//! length of texture data (if m_pData not null)
		unsigned int m_dataLen;
		//! the actual texture data
		SerialPointer<void> m_pData;

	private:
		//! NOT IMPLEMENTED
		GfxTexture(const GfxTexture& other);
		GfxTexture& operator=(const GfxTexture& other);
	};
}

#endif // COSGFXTEXTURE_INCLUDED
