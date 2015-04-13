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


#include "Sampler2D.h"
#include "TextureManager.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(Sampler2D);
TD_ENUM(Sampler2D, Filter, "Min/Mag/Mip filter values");
TD_ENUM_VALUE(Sampler2D, Filter, FILTER_NONE, "No filtering");
TD_ENUM_VALUE(Sampler2D, Filter, FILTER_NEAREST, "Nearest-point filtering");
TD_ENUM_VALUE(Sampler2D, Filter, FILTER_BILINEAR, "Bilinear filtering");
TD_ENUM_VALUE(Sampler2D, Filter, FILTER_ANISOTROPIC, "Anisotropic filtering");
TD_ENUM(Sampler2D, AddressMode, "Texture addressing modes");
TD_ENUM_VALUE(Sampler2D, AddressMode, ADDRMODE_UNSET, "Not set");
TD_ENUM_VALUE(Sampler2D, AddressMode, ADDRMODE_WRAP, "Wrap addressing");
TD_ENUM_VALUE(Sampler2D, AddressMode, ADDRMODE_MIRROR, "Mirror addressing");
TD_ENUM_VALUE(Sampler2D, AddressMode, ADDRMODE_CLAMP, "Clamp addressing");
TD_ENUM_VALUE(Sampler2D, AddressMode, ADDRMODE_BORDER, "Border addressing");
TD_ENUM(Sampler2D, BlendMode, "Texture layer blending mode");
TD_ENUM_VALUE(Sampler2D, BlendMode, BLENDMODE_REPLACE, "Replace pixels from previous layer");
TD_ENUM_VALUE(Sampler2D, BlendMode, BLENDMODE_MODULATE, "Multiply with pixels in previous layer");
TD_ENUM_VALUE(Sampler2D, BlendMode, BLENDMODE_ADD, "Add to pixels in previous layer");
TD_ENUM(Sampler2D, MapHint, "Hints for how to apply texture map");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_UNKNOWN, "Not set");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_DIFFUSE, "Use map for diffuse colors");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_SPECULAR, "Use texture for specular mapping");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_NORMAL, "Use texture for normal mapping");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_AMBIENT, "Use texture for ambient/occlusion mapping");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_ILLUMINATION, "Use texture for light mapping");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_IRRADIANCE, "Use texture for radiance mapping");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_ENVIRONMENT, "Use texture for environment mapping");
TD_ENUM_VALUE(Sampler2D, MapHint, MAP_DISPLACEMENT, "Use texture for displacement mapping");

Sampler2D::Sampler2D()
	: mTexture(0)
{
}

Sampler2D::~Sampler2D()
{
}

bool Sampler2D::initialize()
{
	return true;
}

bool Sampler2D::destroy()
{
	return true;
}

Texture2D* Sampler2D::texture()
{
	if (!mTexture) {
		mTexture = TextureManager::instance().createOrFindInstance2D(getTextureAsset());
	}

	return mTexture;
}
