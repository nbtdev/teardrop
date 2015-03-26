/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
