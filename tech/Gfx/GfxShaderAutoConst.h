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

#if !defined(GFXSHADERAUTOCONST_INCLUDED)
#define GFXSHADERAUTOCONST_INCLUDED

namespace CoS
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

} // namespace CoS

#endif // GFXSHADERAUTOCONST_INCLUDED