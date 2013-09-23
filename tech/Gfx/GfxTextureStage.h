/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXTEXTURESTAGE_INCLUDED)
#define TDGFXTEXTURESTAGE_INCLUDED

#include "Reflection/Reflection.h"

namespace Teardrop
{
	class GfxTexture;
	class GfxMaterial;
	struct Environment;
	class ResourceSerializer;
	class TextureAsset;

	class GfxTextureStage : public Reflection::Object
	{
	public:
		TD_CLASS(GfxTextureStage, Object);

		GfxTextureStage();
		~GfxTextureStage();

		//! initialize the texture stage (anything that can fail)
		bool initialize();
		//! destroy the texture stage
		bool destroy();

		enum Filter
		{
			FILTER_NONE=0,
			FILTER_NEAREST,
			FILTER_BILINEAR,
			FILTER_ANISOTROPIC,
		};

		enum AddressMode
		{
			ADDRMODE_UNSET=0,
			ADDRMODE_WRAP,
			ADDRMODE_MIRROR,
			ADDRMODE_CLAMP,
			ADDRMODE_BORDER,
		};

		enum BlendMode
		{
			BLENDMODE_REPLACE=0,
			BLENDMODE_MODULATE,
			BLENDMODE_ADD,
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
		};

		TD_POINTER_PROPERTY(TextureAsset, "Reference to texture asset to sample", TextureAsset);
		TD_PROPERTY(Enabled, "Enable/disable this texture stage", bool, true, 0);
		TD_PROPERTY(TexCoordSet, "Which texture coordinate set to use on the object rendered with this texture stage", int, 0, 0);
		TD_ENUM_PROPERTY(MinFilter, "Minimisation filter", Filter, FILTER_BILINEAR);
		TD_ENUM_PROPERTY(MagFilter, "Magnification filter", Filter, FILTER_BILINEAR);
		TD_ENUM_PROPERTY(MipMapFilter, "Mipmap filter", Filter, FILTER_BILINEAR);
		TD_ENUM_PROPERTY(AddressModeU, "U coordinate addressing mode", AddressMode, ADDRMODE_WRAP);
		TD_ENUM_PROPERTY(AddressModeV, "V coordinate addressing mode", AddressMode, ADDRMODE_WRAP);
		TD_ENUM_PROPERTY(AddressModeW, "W coordinate addressing mode", AddressMode, ADDRMODE_WRAP);
		TD_ENUM_PROPERTY(BlendMode, "Layer blending mode", BlendMode, BLENDMODE_MODULATE);
		TD_ENUM_PROPERTY(MapHint, "Hint as to purpose for this texture stage", MapHint, MAP_UNKNOWN);
		
		/** accessors
		*/
		//! return texture
		GfxTexture* getTexture();
		//! return texture
		HResource getTextureHandle() const;

		/** mutators
		*/
		//! set stage texture (can be null to disable stage)
		void setTexture(GfxTexture* pTexture, HResource texHandle);

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

#endif // TDGFXTEXTURESTAGE_INCLUDED
