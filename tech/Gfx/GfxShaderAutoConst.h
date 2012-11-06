/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXSHADERAUTOCONST_INCLUDED)
#define GFXSHADERAUTOCONST_INCLUDED

namespace Teardrop
{
	enum AutoConst
	{
		AC_NONE = 0,

		// matrices
		AC_WORLD,
		AC_VIEW,
		AC_PROJ,
		AC_WORLDVIEW,
		AC_WORLDVIEWPROJ,
		AC_VIEWPROJ,
		AC_WORLDVIEWINV,
		AC_WORLDINV,
		AC_VIEWINV,
		AC_WORLDINVTRANS,
		AC_WORLDVIEWINVTRANS,
		AC_WORLDVIEWTRANS,
		AC_LIGHTVIEWPROJ,

		AC_AMBIENT,					// world ambient light value
		AC_CAMERADEPTH,				// the frustum depth for the current snap

		// lights
		AC_LIGHTPOS,				// light world-space position
		AC_LIGHTDIR,				// light world-space direction (normalized)
		AC_LIGHTCOL,				// light diffuse color
		AC_LIGHTSCOL,				// light specular color

		// material colors
		AC_AMBIENTCOL,				// per-material ambient color
		AC_DIFFUSECOL,				// per-material diffuse color
		AC_SPECULARCOL,				// per-material specular color
		AC_EMISSIVECOL,				// per-material emissive color

		// matrix palette (skeletal animation/skinning)
		AC_MATRIXPALETTE,			

		AC_MAX,
		AC_FORCE_DWORD = 0xFFFFFFFF
	};

} // namespace Teardrop

#endif // GFXSHADERAUTOCONST_INCLUDED