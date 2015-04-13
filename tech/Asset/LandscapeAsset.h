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

#if !defined(LANDSCAPEASSET_INCLUDED)
#define LANDSCAPEASSET_INCLUDED

#include "Asset/Asset.h"
#include "Asset/TextureAsset.h"

namespace Teardrop
{
	class LandscapeAsset : public Asset
	{
	public:
		TD_CLASS(LandscapeAsset, Asset);
		TD_CLASS_CREATABLE();
		TD_POINTER_PROPERTY(HeightField, "Landscape terrain heightfield; must be 8- or 16-bit grayscale", TextureAsset, 0);
		TD_PROPERTY(HeightFieldX, "Number of samples in X direction (usually same as heightfield X dimension)", int, 0, "ReadOnly");
		TD_PROPERTY(HeightFieldY, "Number of samples in Y direction (usually same as heightfield Y dimension)", int, 0, "ReadOnly");
		TD_PROPERTY(ColormapX, "Width in texels of colormap (diffuse) texture", int, 0, "ReadOnly");
		TD_PROPERTY(ColormapY, "Height in texels of colormap (diffuse) texture", int, 0, "ReadOnly");
		TD_PROPERTY(MinAltitude, "World-space altitude of the lowest sample in the heightfield (can be negative)", float, -1, 0);
		TD_PROPERTY(MaxAltitude, "World-space altitude of the highest sample in the heightfield (can be negative)", float, 1, 0);
		TD_PROPERTY(Scale, "Spacing, in world units, of heightfield samples", float, 1, 0);
		TD_POINTER_PROPERTY(DiffuseMap, "Landscape terrain colormap", TextureAsset, 0);
		TD_POINTER_PROPERTY(AttributesMap, "Landscape terrain attributes map", TextureAsset, 0);
		TD_POINTER_PROPERTY(LightMap, "Landscape terrain light map", TextureAsset, 0);
		TD_POINTER_PROPERTY(SpecularLightMap, "Landscape terrain specular highlight map", TextureAsset, 0);
		TD_POINTER_PROPERTY(ShadowMap, "Landscape terrain shadow map", TextureAsset, 0);
		TD_POINTER_PROPERTY(NormalMap, "Landscape terrain normal map", TextureAsset, 0);

		LandscapeAsset();
		~LandscapeAsset();

		TD_DECLARE_ALLOCATOR();

	protected:
	};
} // namespace Teardrop

#endif // LANDSCAPEASSET_INCLUDED
