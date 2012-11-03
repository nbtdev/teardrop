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

#if !defined(COSGFXSHADERCODE_INCLUDED)
#define COSGFXSHADERCODE_INCLUDED

namespace CoS
{
	struct GfxShaderCode
	{
		enum
		{
			VS_2_0=0,
			VS_3_0,
			VS_4_0,
			MAX_VS_PROFILES
		};

		enum
		{
			PS_2_0=0,
			PS_3_0,
			PS_4_0,
			MAX_PS_PROFILES
		};

		enum
		{
			VS_GLOBALS=0,
			VS_IN_OPEN,
			VS_IN_CLOSE,
			VS_MAIN_OPEN,
			VS_MAIN_CLOSE,
			VS_WVP_TRANSFORM,
			VS_POS_SKINNED,
			VS_WVP_POS,
			VS_SKIN_POS,
			VS_NORMAL_TRANSFORM,
			VS_NORM_SKINNED,
			VS_WVP_NORM,
			VS_SKIN_NORM,
			VS_TEXCOORD_TRANSFORM,
			VS_COPY_COLOR,
			VS_CALC_LIGHT_VEC,

			VS_TERRAIN_CHUNK,
			VS_MACRO,
			VS_PROP,
			VS_VFX,

			MAX_VS_SNIPPETS
		};

		enum
		{
			PS_GLOBALS=0,
			PS_IN_OPEN,
			PS_IN_CLOSE,
			PS_MAIN_OPEN,
			PS_MAIN_CLOSE,
			PS_SAMPLE_2D,
			PS_SAMPLE_3D,
			PS_SAMPLE_CUBE,
			PS_BLINN_LIGHTING,
			PS_CALC_BLINN,

			PS_TERRAIN_CHUNK,
			PS_MACRO,
			PS_PROP,
			PS_VFX,

			MAX_PS_SNIPPETS
		};

		static const char* vs[MAX_VS_PROFILES][MAX_VS_SNIPPETS];
		static const char* ps[MAX_PS_PROFILES][MAX_PS_SNIPPETS];

		enum
		{
			INVARIANT_VSCONSTANTS_F_START = 0,
			VARIANT_VSCONSTANTS_F_START = 12,
			BONE_CONSTANTS_START = 32,
		};

		enum
		{
			INVARIANT_VSCONSTANTS_F_COUNT = 12,
			VARIANT_VSCONSTANTS_F_COUNT = 20,
			BONE_CONSTANTS_COUNT = 224,
		};

		enum
		{
			INVARIANT_PSCONSTANTS_F_START = 4,
			VARIANT_PSCONSTANTS_F_START = 28,
		};

		enum
		{
			INVARIANT_PSCONSTANTS_F_COUNT = 9,
			VARIANT_PSCONSTANTS_F_COUNT = 4,
		};

		enum
		{
			// leave room at the bottom for shader constants
			VIEWPROJ		= 4,
			LIGHT0POS		= 8,

			WORLD			= 12,
			WVP				= 16,
			VI				= 20,
			TEXMAT			= 24,
			LIGHT0DIR		= 32,
			VIEW			= 36,

			MATRIXPALETTE	= 40,
		};

		enum
		{
			LIGHT0COL		= 4,
			LIGHT0SCOL		= 8,
			AMBIENT			= 12,

			DIFFUSE_COL		= 28,
			AMBIENT_COL		= 29,
			SPECULAR_COL	= 30,
			EMISSIVE_COL	= 31,
		};
	};
}

#endif // COSGFXSHADERCODE_INCLUDED
