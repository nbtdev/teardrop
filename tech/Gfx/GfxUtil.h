/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXUTIL_INCLUDED)
#define TDGFXUTIL_INCLUDED

#include "GfxCommon.h"

namespace Teardrop
{
	class GfxMesh;
	class GfxRenderer;
	struct Environment;
	class Vector4;
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

#endif // TDGFXUTIL_INCLUDED
