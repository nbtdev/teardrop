/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTUREASSET_INCLUDED)
#define TEXTUREASSET_INCLUDED

#include "Asset/Asset.h"

namespace Teardrop
{
	class TextureAsset : public Asset
	{
	public:
		TD_CLASS(TextureAsset, Asset);
		TD_CLASS_CREATABLE();
		TD_PROPERTY(Width, "Texture width in pixels", int, 0, "ReadOnly");
		TD_PROPERTY(Height, "Texture height in pixels", int, 0, "ReadOnly");

		enum Type {
			TEXTYPE_1D,
			TEXTYPE_2D,
			TEXTYPE_3D
		};

		enum Format {
			TEXFMT_R,
			TEXFMT_RG,
			TEXFMT_RGB,
			TEXFMT_ARGB,
			TEXFMT_R_F32,
			TEXFMT_RG_F32,
			TEXFMT_RGB_F32,
			TEXFMT_ARGB_F32,
			TEXFMT_BC1,
			TEXFMT_BC2,
			TEXFMT_BC3,
			TEXFMT_PVRTC,
		};

		TD_ENUM_PROPERTY(Type, "Texture Type", Type, TEXTYPE_2D);
		TD_ENUM_PROPERTY(Format, "Texture Format", Format, TEXFMT_ARGB);
		TD_PROPERTY(GenerateMipmaps, "Generate mipmaps for texture asset at runtime", bool, false, 0);


		TextureAsset();
		~TextureAsset();

		int serialize(Stream& strm);
		int deserialize(Stream& strm);

		TD_DECLARE_ALLOCATOR();

	protected:
	};
} // namespace Teardrop

#endif // TEXTUREASSET_INCLUDED
