/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
