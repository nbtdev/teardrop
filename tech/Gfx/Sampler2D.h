/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(SAMPLER2D_INCLUDED)
#define SAMPLER2D_INCLUDED

#include "Asset/TextureAsset.h"
#include "Reflection/Reflection.h"

namespace Teardrop
{
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
