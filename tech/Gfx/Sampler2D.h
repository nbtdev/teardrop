/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SAMPLER2D_INCLUDED)
#define SAMPLER2D_INCLUDED

#include "Reflection/Reflection.h"

namespace Teardrop
{
	class TextureAsset;

	namespace Gfx
	{
		class Texture2D;

		class Sampler2D : public Reflection::Object
		{
		public:
			TD_CLASS(Sampler2D, Object);
			TD_CLASS_CREATABLE();
			TD_POINTER_PROPERTY(TextureAsset, "Texture Asset", TextureAsset, 0);

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

			TD_PROPERTY(Enabled, "Enable/disable this sampler", bool, true, 0);
			TD_PROPERTY(TexCoordSet, "Which texture coordinate set to use on the object rendered with this sampler", int, 0, 0);
			TD_ENUM_PROPERTY(MinFilter, "Minimisation filter", Filter, FILTER_BILINEAR);
			TD_ENUM_PROPERTY(MagFilter, "Magnification filter", Filter, FILTER_BILINEAR);
			TD_ENUM_PROPERTY(MipMapFilter, "Mipmap filter", Filter, FILTER_BILINEAR);
			TD_ENUM_PROPERTY(AddressModeU, "U coordinate addressing mode", AddressMode, ADDRMODE_WRAP);
			TD_ENUM_PROPERTY(AddressModeV, "V coordinate addressing mode", AddressMode, ADDRMODE_WRAP);
			TD_ENUM_PROPERTY(AddressModeW, "W coordinate addressing mode", AddressMode, ADDRMODE_WRAP);
			TD_ENUM_PROPERTY(BlendMode, "Layer blending mode", BlendMode, BLENDMODE_MODULATE);

			Sampler2D();
			~Sampler2D();

			bool initialize();
			bool destroy();

			Texture2D* texture();

			TD_DECLARE_ALLOCATOR();

		protected:
			Texture2D* mTexture;
		};
	}
}

#endif // SAMPLER2D_INCLUDED
