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

#if !defined(COSGFXTEXTURESTAGE_INCLUDED)
#define COSGFXTEXTURESTAGE_INCLUDED

namespace CoS
{
	class GfxTexture;
	class GfxMaterial;
	struct Environment;
	class ResourceSerializer;

	class GfxTextureStage
	{
		DECLARE_SERIALIZABLE(GfxTextureStage);

	public:
		//! normal c'tor (cannot fail)
		GfxTextureStage();
		//! placement c'tor (cannot fail)
		GfxTextureStage(int);
		//! d'tor (cannot fail)
		~GfxTextureStage();

		//! initialize the texture stage (anything that can fail)
		bool initialize();
		//! destroy the texture stage
		bool destroy();

		enum Filter
		{
			NONE=0,
			NEAREST,
			BILINEAR,
			ANISOTROPIC,

			FILTER_FORCE_SIZE=0x000000FF,
		};

		enum AddressMode
		{
			UNSET=0,
			WRAP,
			MIRROR,
			CLAMP,
			BORDER,

			ADDRMODE_FORCE_SIZE=0x000000FF,
		};

		enum BlendMode
		{
			REPLACE=0,
			MODULATE,
			ADD,

			BLENDMODE_FORCE_SIZE=0x000000FF,
		};

		// map hints
		enum MapHint
		{
			MAP_UNKNOWN,
			MAP_DIFFUSE,
			MAP_SPECULAR,
			MAP_NORMAL,
			MAP_AMBIENT,
			MAP_ILLUMINATION,
			MAP_IRRADIANCE,
			MAP_ENVIRONMENT,
			MAP_DISPLACEMENT,

			HINT_FORCE_SIZE=0x000000FF,
		};
		
		/** accessors
		*/
		//! return texture
		GfxTexture* getTexture();
		//! return texture
		HResource getTextureHandle() const;
		//! return texture filtering settings (minification, magnification, mip)
		void getFilter(Filter& minFilter, Filter& magFilter, Filter& mipFilter) const;
		//! return texture addressing modes (U, V, W)
		void getTextureAddressing(AddressMode& U, AddressMode& V, AddressMode& W) const;
		//! return layer blend mode
		BlendMode getLayerBlendMode() const;
		//! return map usage hint
		MapHint getMapHint() const;
		//! check if stage is enabled
		bool isEnabled() const;
		//! which texcoord set to use?
		unsigned int getTexCoordSet() const;

		/** mutators
		*/
		//! set stage texture (can be null to disable stage)
		void setTexture(GfxTexture* pTexture, HResource texHandle);
		//! set texture filtering (min, mag, mip)
		void setFilter(Filter minFilter, Filter magFilter, Filter mipFilter);
		//! set texture addressing modes (U, V, W)
		void setTextureAddressing(AddressMode U, AddressMode V, AddressMode W);
		//! set layer blend mode
		void setLayerBlendMode(BlendMode mode);
		//! set map usage hint
		void setMapHint(MapHint hint);
		//! turn stage on or off
		void setEnabled(bool bEnabled);
		//! which texcoord set to use?
		void setTexCoordSet(unsigned int set);

		/**
			Serialization
		*/
		//! package for storage
		bool serialize(ResourceSerializer& serializer);
		//! set texture name (meaningful only during serialization)
		void setTextureName(const char* name);
		//! get texture name (meaningful only during serialization)
		const char* getTextureName() const;

		DECLARE_GFX_ALLOCATOR();

	private:
		unsigned int m_borderColor;
		Filter m_minFilter;
		Filter m_magFilter;
		Filter m_mipFilter;
		MapHint m_mapHint;
		BlendMode m_blendMode;
		AddressMode m_addrU;
		AddressMode m_addrV;
		AddressMode m_addrW;
		SerialPointer<void> m_hTexture; // this is actually an HResource
		unsigned int m_enabled;
		unsigned int m_pName; // used only during serialization
		unsigned int m_texCoordSet;
		SerialPointer<GfxTexture> m_pTexture;

		//! NOT IMPLEMENTED
		GfxTextureStage(const GfxTextureStage& other);
		GfxTextureStage& operator=(const GfxTextureStage& other);
	};
}

#endif // COSGFXTEXTURESTAGE_INCLUDED
