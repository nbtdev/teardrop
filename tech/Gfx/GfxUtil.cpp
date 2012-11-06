/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxUtil.h"
#include "GfxCamera.h"
#include "GfxLight.h"
#include "GfxMesh.h"
#include "GfxSubMesh.h"
#include "GfxVertexData.h"
#include "GfxIndexData.h"
#include "GfxVertexFormat.h"
#include "GfxTexture.h"
#include "Math/Transform.h"
#include "Math/Ray.h"
#include "Math/Triangle.h"
#include "Math/Plane.h"
#include "Math/Vector4.h"
#include "Math/Query.h"
#include "Math/MathUtil.h"
#include "Util/FourCC.h"
#include <assert.h>
#include <new.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
size_t GfxUtil::getSizeOf(Teardrop::VertexElementType type)
{
	switch(type)
	{
	case FLOAT:
		return sizeof(float);
		break;

	case FLOAT2:
		return sizeof(float) * 2;
		break;

	case FLOAT3:
		return sizeof(float) * 3;
		break;

	case FLOAT4:
		return sizeof(float) * 4;
		break;

	case HALF2:
		return sizeof(short) * 2;
		break;

	case HALF4:
		return sizeof(short) * 4;
		break;

	case BYTE4:
	case VET_COLOR:
		return sizeof(char) * 4;
		break;
	}

	return 0;
}
//---------------------------------------------------------------------------
extern bool createTeapotImpl(GfxMesh&, Environment&);
//---------------------------------------------------------------------------
bool GfxUtil::createTeapot(GfxMesh& teapotMesh, Environment& env)
{
	return createTeapotImpl(teapotMesh, env);
}
//---------------------------------------------------------------------------
unsigned int GfxUtil::packColor(
	unsigned char r, 
	unsigned char g,
	unsigned char b,
	unsigned char a
	)
{
	return 
		(a << 24) | (r << 16) | (g << 8) | (b);
}
//---------------------------------------------------------------------------
unsigned int GfxUtil::makePackedColor(float r, float g, float b, float a)
{
	return packColor(
		(unsigned char)(255.f * r),
		(unsigned char)(255.f * g),
		(unsigned char)(255.f * b),
		(unsigned char)(255.f * a)
		);
}
//---------------------------------------------------------------------------
unsigned int GfxUtil::makePackedColor(const Vector4& c)
{
	return makePackedColor(c.x, c.y, c.z, c.w);
}
//---------------------------------------------------------------------------
void GfxUtil::unpackColor(unsigned int c, float& r, float& g, float& b, float& a)
{
	float d = 1 / 255.f;
	a = (float)(0x000000FF & c) * d;
	c >>= 8;
	b = (float)(0x000000FF & c) * d;
	c >>= 8;
	g = (float)(0x000000FF & c) * d;
	c >>= 8;
	r = (float)(0x000000FF & c) * d;
}
//---------------------------------------------------------------------------
void GfxUtil::unpackColor(unsigned int c, Vector4& r)
{
	unpackColor(c, r.x, r.y, r.z, r.w);
}
//---------------------------------------------------------------------------
void GfxUtil::unpackColor(
	unsigned int c, 
	unsigned char& r, 
	unsigned char& g, 
	unsigned char& b, 
	unsigned char& a)
{
	a = (unsigned char)(0x000000FF & c);
	c >>= 8;
	b = (unsigned char)(0x000000FF & c);
	c >>= 8;
	g = (unsigned char)(0x000000FF & c);
	c >>= 8;
	r = (unsigned char)(0x000000FF & c);
}
//---------------------------------------------------------------------------
void* GfxUtil::createResource(const FourCC& fourCC)
{
	void* pMem = 0;

	if (fourCC == GfxMesh::RESOURCE_TYPE)
	{
		return TD_NEW GfxMesh;
	}

	if (fourCC == GfxTexture::RESOURCE_TYPE)
	{
		return TD_NEW GfxTexture;
	}

	return pMem;
}
//---------------------------------------------------------------------------
bool GfxUtil::collideRayWithMesh(
   /*in*/ const Ray& ray,			// test ray
   /*in*/ const Transform& xform,	// world transform for GfxMesh being tested
   /*in*/ const GfxMesh& mesh,		// mesh to test against
   /*out*/float& t					// parameter (distance) along ray of nearest collision point
)
{
	// go through the vert buffers for the selected gizmo
	Vector4 pos[3];
	const Quaternion& q = xform.rot;
	const Vector4& p = xform.trans;
	const Vector4& s = xform.scale;

	// we only need to test against the upper three buffers; the lower three
	// are the "outline" buffers and testing against them is a waste
	float distToIntersect = MathUtil::POSITIVE_INFINITY;
	int nearestTri = 0;

	size_t numSM = mesh.getNumSubMeshes();
	for (size_t i=0; i<numSM; ++i)
	{
		GfxSubMesh* pSM = mesh.getSubMesh(i);

		// where does the position data exist?
		GfxVertexFormat fmt = pSM->getVertexFormat();
		const GfxVertexFormat::Element& elem = fmt.getElementBySemantic(POSITION);
		size_t offset = size_t(elem.offset);
		size_t stride = fmt.getVertexSize();
		GfxVertexData* pVD = pSM->getVertexData(elem.getSource());
		char* pVData = (char*)pVD->getBuffer(); // assumes that there is one...
		if (!pVData)
			continue;

		// only support trilists right now
		if (pSM->getPrimitiveType() != TRILIST)
			continue;

		// go through the index buffer and pick out the triangles
		GfxIndexData* pID = pSM->getIndexData();
		size_t idxCount = pID->getIndexCount();
		char* pIData = (char*)pID->getBuffer();
		if (!pIData)
			continue;

		size_t indexStride = pID->getIndexSize();
		for (size_t j=0; j<idxCount; ++j)
		{
			// get next three indices
			size_t i0, i1, i2;
			if (indexStride == 2)
			{
				// 16-bit indices, read them as ushorts
				i0 = size_t(*((unsigned short*)pIData)); pIData += indexStride;
				i1 = size_t(*((unsigned short*)pIData)); pIData += indexStride;
				i2 = size_t(*((unsigned short*)pIData)); pIData += indexStride;
			}
			else
			{
				// 16-bit indices, read them as uints
				i0 = size_t(*((unsigned int*)pIData)); pIData += indexStride;
				i1 = size_t(*((unsigned int*)pIData)); pIData += indexStride;
				i2 = size_t(*((unsigned int*)pIData)); pIData += indexStride;
			}

			// from these, get vertex position data
			Vector4& p0 = (Vector4&)*((Vector4*)(pVData + i0 * stride + offset));
			Vector4& p1 = (Vector4&)*((Vector4*)(pVData + i1 * stride + offset));
			Vector4& p2 = (Vector4&)*((Vector4*)(pVData + i2 * stride + offset));

			// transform these into world space
			Vector4 t0(q * (p0.applyScale(s)) + p);
			Vector4 t1(q * (p1.applyScale(s)) + p);
			Vector4 t2(q * (p2.applyScale(s)) + p);

			// find out where/if the ray intersects this triangle
			float _t;
			if (Query::Ray_Triangle(ray, Triangle(t0, t1, t2), _t))
			{
				// record the nearest one only
				Vector4 pt(ray.end + ray.dir * _t);
				float d = (pt - ray.end).getLength2();
				if (d < distToIntersect)
				{
					distToIntersect = d;
					nearestTri = i;
					t = _t;
				}
			}
		}
	}

	return (nearestTri != 0);

#if 0
	for (int i=3; i<6; ++i)
	{
		const Vert* pData = m_pVertData[m_currentGizmo][i];
		if (!pData)
			continue;

		// the only difference in how we interpret trifan and tristrip is 
		// how we deal with "p0" and "p1"; "p2" is always (beyond the second
		// vert) the "next" vert in the primitive
		Vector4 p0(*((Vector4*)&pData[0].x));
		Vector4 p1(*((Vector4*)&pData[1].x));
		p0.w = 0; // since the vert is actually a 3-vector, and .w is actually
		p1.w = 0; // the first color component, which is non-zero
		pos[0] = q * (p0.applyScale(s)) + p;
		pos[1] = q * (p1.applyScale(s)) + p;

		unsigned int verts = pMesh->getSubMesh(i)->getVertexData(0)->getVertexCount();
		for (size_t k=2; k<verts; ++k)
		{
			Vector4 p2(*((Vector4*)&pData[k].x));
			p2.w = 0;
			pos[2] = q * (p2.applyScale(s)) + p;

			// check the tri for collision
			float t;
			if (Query::Ray_Triangle(ray, Triangle(pos[0], pos[1], pos[2]), t))
			{
				// generate the bitfield that corresponds to what actually is active;
				// each of the 9 VertData elements corresponds ordinally to the list of
				// operation types in the enum in the header; this means all we need
				// to do is, if one triangle intersects, mark the fact with a bitshift 
				// in m_operations and move onto the next VertData element

				// record the nearest one only
				Vector4 pt(ray.end + ray.dir * t);
				float d = (pt - ray.end).getLength2();
				if (d < distToIntersect)
				{
					distToIntersect = d;
					nearestTri = i;
				}
			}

			// reset for next iteration
			pos[1] = pos[2];
		}
	}

	// highlight the active operations
	bool rtn = false;
	if (nearestTri)
	{
		nearestTri -= 3;
		m_operations |= (1 << (nearestTri + (m_currentGizmo*3)));
		intersectPoint = ray.end + ray.dir * MathUtil::sqrt(distToIntersect);
		rtn = true;
	}
#endif 
}
//---------------------------------------------------------------------------
GfxCamera* GfxUtil::createShadowCamera(
	GfxCamera* pViewCam, GfxLight* pLight)
{
	pViewCam;
	GfxCamera* pShadowCam = TD_NEW GfxCamera;
	pShadowCam->initialize();
#if 0
	// the view camera defines what the user can see, so for light types that 
	// do not define a natural frustum (spotlight), we need to synthesize
	// a "view" frustum for the light's POV, that fits well with the type of 
	// light
	if (pLight->getType() == GfxLight::LT_DIRECTIONAL)
	{
		// figure out the width and height of the "view" volume, 
		// orthographic projection
	}
	// use a perspective projection of some sort
	else if (pLight->getType() == GfxLight::LT_POINT)
	{
		// these are just test values for now
		pShadowCam->setFovY(MathUtil::HALF_PI);
		pShadowCam->setNearClip(1);
		pShadowCam->setFarClip(500);
		pShadowCam->setAspect(1);
		pShadowCam->setPosition(pLight->getPosition());

		// very very naive and temp; just have it look at the center of the view frustum,
		// which we can get from the cam orientation (ray) and its near and far plane
		// intersections
		const Plane* pPlanes = pViewCam->getFrustumPlanes(); // near and far are index 4 and 5
		const Plane& nearPlane = pPlanes[4];
		const Plane& farPlane = pPlanes[5];
		Quaternion q(pViewCam->getOrientation());
		Ray cameraRay(pShadowCam->getPosition(), q * Vector4::UNIT_Z);

		float t = -(dot(nearPlane.n, cameraRay.end) + nearPlane.d) / (dot(nearPlane.n, cameraRay.dir));
		Vector4 nearP(cameraRay.dir * t);
		t = -(dot(farPlane.n, cameraRay.end) + farPlane.d) / (dot(farPlane.n, cameraRay.dir));
		Vector4 farP(cameraRay.dir * t);
		Vector4 center((farP + nearP) * 0.5f);

		pShadowCam->setLookAt(center);
	}
	else // spot
	{
	}

	pShadowCam->update();
#endif
	pLight->setShadowCamera(pShadowCam);
	return pShadowCam;
}
