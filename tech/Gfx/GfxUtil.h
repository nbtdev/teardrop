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

#if !defined(COSGFXUTIL_INCLUDED)
#define COSGFXUTIL_INCLUDED

#include "GfxCommon.h"

namespace CoS
{
	class GfxMesh;
	class GfxRenderer;
	struct Environment;
	class Vector4;
	struct FourCC;
	class Ray;
	class Transform;
	class GfxCamera;
	class GfxLight;

	/** class container for utility methods
	*/
	class GfxUtil
	{
	public:
		//! get the size in bytes of the indicated type ID
		static size_t getSizeOf(VertexElementType type);
		static bool createTeapot(GfxMesh& teapotMesh, Environment& env);
		static unsigned int makePackedColor(float r, float g, float b, float a);
		static unsigned int makePackedColor(const Vector4& color);
		static unsigned int packColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		static void unpackColor(unsigned int c, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);
		static void unpackColor(unsigned int c, float& r, float& g, float& b, float& a);
		static void unpackColor(unsigned int c, Vector4& out);
		static void* createResource(const FourCC& fourC);
		static GfxMesh* createAABBTemplateMesh(GfxRenderer* pRend);
		// returns true if ray collides with mesh, with ray collision parameter in t; false if no collision
		static bool collideRayWithMesh(
			/*in*/ const Ray& ray,			// test ray in world space
			/*in*/ const Transform& xform,	// world transform for GfxMesh being tested
			/*in*/ const GfxMesh& mesh,		// mesh to test
			/*out*/float& t					// parameter (distance) along ray of nearest collision point
			);
		static GfxCamera* createShadowCamera(GfxCamera* pViewCam, GfxLight* pShadowCastingLight);
	};
}

#endif // COSGFXUTIL_INCLUDED
