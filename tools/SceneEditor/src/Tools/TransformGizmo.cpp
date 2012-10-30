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

#include "stdafx.h"
#include "TransformGizmo.h"
#include "Game/include/ZoneObject.h"
#include "Gfx/include/GfxCamera.h"
#include "Gfx/include/GfxMesh.h"
#include "Gfx/include/GfxSubMesh.h"
#include "Gfx/include/GfxVertexData.h"
#include "Gfx/include/GfxIndexData.h"
#include "Gfx/include/GfxVertexFormat.h"
#include "Gfx/include/GfxMaterial.h"
#include "Gfx/include/GfxUtil.h"
#include "Math/include/Vector4.h"
#include "Math/include/MathUtil.h"
#include "Math/include/Triangle.h"
#include "Math/include/Ray.h"
#include "Math/include/Query.h"
#include "Resource/include/ResourceManager.h"
#include "Util/include/Environment.h"
#include "Memory/include/Memory.h"
#include "Memory/include/Allocators.h"
#include <assert.h>

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace CoS;
//---------------------------------------------------------------------------
// CONSTANTS
const float GIZMO_TRANSPARENCY = 0.1f;
const Vector4 s_xColour(1, 0, 0, 1);
const Vector4 s_yColour(0, 1, 0, 1);
const Vector4 s_zColour(0, 0, 1, 1);
const static Vector4 s_ArrowSelectColour = Vector4(1, 1, 0, 1);
const static Vector4 s_xArrowColour = Vector4(1, 0, 0, 1);
const static Vector4 s_yArrowColour = Vector4(0, 1, 0, 1);
const static Vector4 s_zArrowColour = Vector4(0, 0, 1, 1);
const static Vector4 s_xScaleColour = Vector4(1, 0, 0, GIZMO_TRANSPARENCY);
const static Vector4 s_yScaleColour = Vector4(0, 1, 0, GIZMO_TRANSPARENCY);
const static Vector4 s_zScaleColour = Vector4(0, 0, 1, GIZMO_TRANSPARENCY);
const static Vector4 s_xRotateColour = Vector4(1, 0, 0, GIZMO_TRANSPARENCY);
const static Vector4 s_yRotateColour = Vector4(0, 1, 0, GIZMO_TRANSPARENCY);
const static Vector4 s_zRotateColour = Vector4(0, 0, 1, GIZMO_TRANSPARENCY);
const static Vector4 s_clearAlpha = Vector4(1, 1, 1, 0);

// rotation circle radius 
static const float s_radius = 4.0f; 
// length of translate arrows from center
static const float s_transHeight = 4.5f; 
// height of scale triangles
static const float s_scaleHeight = 2.f; 
// number of degrees per line in circle
static float s_radialGranularity;
//---------------------------------------------------------------------------
struct Vert
{
	float x, y, z;
	unsigned char a, b, g, r;
};
//---------------------------------------------------------------------------
TransformGizmo::TransformGizmo()
{
	m_pObj = 0;
	m_gizmo[0] = INVALID_RESOURCE_HANDLE;
	m_gizmo[1] = INVALID_RESOURCE_HANDLE;
	m_gizmo[2] = INVALID_RESOURCE_HANDLE;
	m_pMtl = m_pMtlTrans = 
		m_pMtlSelected = m_pMtlTransSelected = 0;
	m_bVisible = false;
	m_bFreezeOperation = false;
	s_radialGranularity = 1.0f / MathUtil::TWO_PI; 
	m_currentGizmo = GIZMO_POSITION;
	m_operations = OP_NONE;
	memset(m_pVertData, 0, sizeof(m_pVertData));
}
//---------------------------------------------------------------------------
TransformGizmo::TransformGizmo(ZoneObject* pObj)
{
	assert(pObj);

	m_pObj = pObj;
	m_gizmo[0] = INVALID_RESOURCE_HANDLE;
	m_gizmo[1] = INVALID_RESOURCE_HANDLE;
	m_gizmo[2] = INVALID_RESOURCE_HANDLE;
	m_pMtl = m_pMtlTrans = 
		m_pMtlSelected = m_pMtlTransSelected = 0;
	m_bVisible = false;
	m_bFreezeOperation = false;
	s_radialGranularity = 1.0f / MathUtil::TWO_PI; 
	m_currentGizmo = GIZMO_POSITION;
	m_operations = OP_NONE;
	memset(m_pVertData, 0, sizeof(m_pVertData));
}
//---------------------------------------------------------------------------
TransformGizmo::~TransformGizmo()
{
}
//---------------------------------------------------------------------------
bool TransformGizmo::isInitialized()
{
	return (m_gizmo[0] != INVALID_RESOURCE_HANDLE);
}
//---------------------------------------------------------------------------
float TransformGizmo::getRadius() const
{
	switch(m_currentGizmo)
	{
	case GIZMO_POSITION:
	case GIZMO_SCALE:
		return s_transHeight;

	case GIZMO_ROTATION:
		return s_radius;
	}

	return MathUtil::NEGATIVE_INFINITY;
}
//---------------------------------------------------------------------------
bool TransformGizmo::createPositionGizmo(GfxVertexFormat& fmt)
{
	/*
		Rotation Gizmo
	*/

	GfxMesh* pGizmo = AutoResPtr<GfxMesh>(m_gizmo[GIZMO_POSITION]);
	
	if (!pGizmo)
		return false;

	/*
		X-AXIS POSITION AXIS
	*/

	// draw a line for the x axis
	GfxSubMesh* pSM = pGizmo->createSubMesh();
	size_t strm;
	GfxVertexData* pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		2,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));

	pSM->setPrimitiveType(LINESTRIP);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	Vert* pBuf = m_pVertData[GIZMO_POSITION][0] = (Vert*)pVD->getBuffer();

	unsigned int col = GfxUtil::makePackedColor(s_xColour);
	pBuf->x = -s_transHeight;
	pBuf->y = pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;
	pBuf->x = s_transHeight;
	pBuf->y = pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;

	pVD->updateData();

	// make an instance of this mesh
	m_inst[0][GIZMO_POSITION].setMeshHandle(m_gizmo[GIZMO_POSITION]);
	m_inst[1][GIZMO_POSITION].setMeshHandle(m_gizmo[GIZMO_POSITION]);


	// and one for the y axis
	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		2,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(LINESTRIP);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	pBuf = m_pVertData[GIZMO_POSITION][1] = (Vert*)pVD->getBuffer();

	col = GfxUtil::makePackedColor(s_yColour);
	pBuf->y = -s_transHeight;
	pBuf->x = pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;
	pBuf->y = s_transHeight;
	pBuf->x = pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;

	pVD->updateData();

	// and one for the z axis
	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		2,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(LINESTRIP);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	pBuf = m_pVertData[GIZMO_POSITION][2] = (Vert*)pVD->getBuffer();

	col = GfxUtil::makePackedColor(s_zColour);
	pBuf->z = -s_transHeight;
	pBuf->x = pBuf->y = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;
	pBuf->z = s_transHeight;
	pBuf->x = pBuf->y = 0;
	*((unsigned int*)&pBuf->a) = col;

	pVD->updateData();

	// and now the arrows at the positive ends of each axis line...x first
	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		91,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(TRIFAN);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	pBuf = m_pVertData[GIZMO_POSITION][3] = (Vert*)pVD->getBuffer();

	col = GfxUtil::makePackedColor(s_xColour);
	pBuf->x = s_transHeight + 1.5f;
	pBuf->y = pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;

	float d = 0;
	for (size_t i=0; i<90; ++i)
	{
		pBuf->x = s_transHeight;
		pBuf->y = 0.5f * MathUtil::cos(d);
		pBuf->z = 0.5f * MathUtil::sin(d);
		*((unsigned int*)&pBuf->a) = col;
		d += s_radialGranularity * 4;
		++pBuf;
	}

	pVD->updateData();

	// and then y...
	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		91,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(TRIFAN);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	pBuf = m_pVertData[GIZMO_POSITION][4] = (Vert*)pVD->getBuffer();

	col = GfxUtil::makePackedColor(s_yColour);
	pBuf->y = s_transHeight + 1.5f;
	pBuf->x = pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;

	d = 0;
	for (size_t i=0; i<90; ++i)
	{
		pBuf->y = s_transHeight;
		pBuf->x = 0.5f * MathUtil::cos(d);
		pBuf->z = 0.5f * MathUtil::sin(d);
		*((unsigned int*)&pBuf->a) = col;
		d += s_radialGranularity * 4;
		++pBuf;
	}

	pVD->updateData();

	// and then z...
	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		91,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(TRIFAN);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	pBuf = m_pVertData[GIZMO_POSITION][5] = (Vert*)pVD->getBuffer();

	col = GfxUtil::makePackedColor(s_zColour);
	pBuf->z = s_transHeight + 1.5f;
	pBuf->x = pBuf->y = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;

	d = 0;
	for (size_t i=0; i<90; ++i)
	{
		pBuf->z = s_transHeight;
		pBuf->x = 0.5f * MathUtil::cos(d);
		pBuf->y = 0.5f * MathUtil::sin(d);
		*((unsigned int*)&pBuf->a) = col;
		d += s_radialGranularity * 4;
		++pBuf;
	}

	pVD->updateData();

	// make an instance of this mesh
	m_inst[0][GIZMO_POSITION].setMeshHandle(m_gizmo[GIZMO_POSITION]);
	m_inst[1][GIZMO_POSITION].setMeshHandle(m_gizmo[GIZMO_POSITION]);

	return true;
}
//---------------------------------------------------------------------------
bool TransformGizmo::createRotationGizmo(GfxVertexFormat& fmt)
{
	/*
		Rotation Gizmo
	*/

	GfxMesh* pGizmo = AutoResPtr<GfxMesh>(m_gizmo[GIZMO_ROTATION]);

	/*
		X-AXIS ROTATION CIRCLE
	*/

	GfxSubMesh* pSM = pGizmo->createSubMesh();
	size_t strm;
	GfxVertexData* pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		361,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(LINESTRIP);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	Vert* pBuf = m_pVertData[GIZMO_ROTATION][0] = (Vert*)pVD->getBuffer();

	// draw circle around the X axis (in the Y and Z axes)
	pBuf->x = 0;
	pBuf->y = 0;
	pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = GfxUtil::makePackedColor(s_xColour);
	++pBuf;

	float d = 0;
	for (size_t i=1; i<361; ++i)
	{
		pBuf->x = 0;
		pBuf->y = s_radius * MathUtil::cos(d);
		pBuf->z = s_radius * MathUtil::sin(d);
		d += s_radialGranularity;

		*((unsigned int*)&pBuf->a) = GfxUtil::makePackedColor(s_xColour);
		++pBuf;
	}

	pVD->updateData();

	/*
		Y-AXIS ROTATION CIRCLE
	*/

	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		361,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(LINESTRIP);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	pBuf = m_pVertData[GIZMO_ROTATION][1] = (Vert*)pVD->getBuffer();

	// draw circle around the Y axis (in the X and Z axes)
	pBuf->x = 0;
	pBuf->y = 0;
	pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = GfxUtil::makePackedColor(s_yColour);
	++pBuf;

	d = 0;
	for (size_t i=1; i<361; ++i)
	{
		pBuf->x = s_radius * MathUtil::sin(d);
		pBuf->y = 0;
		pBuf->z = s_radius * MathUtil::cos(d);
		d += s_radialGranularity;

		*((unsigned int*)&pBuf->a) = GfxUtil::makePackedColor(s_yColour);
		++pBuf;
	}

	pVD->updateData();

	/*
		Z-AXIS ROTATION CIRCLE
	*/

	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		361,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(LINESTRIP);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtl);
	pBuf = m_pVertData[GIZMO_ROTATION][2] = (Vert*)pVD->getBuffer();

	// draw circle around the Z axis (in the X and Y axes)
	pBuf->x = 0;
	pBuf->y = 0;
	pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = GfxUtil::makePackedColor(s_zColour);
	++pBuf;

	d = 0;
	for (size_t i=1; i<361; ++i)
	{
		pBuf->x = s_radius * MathUtil::cos(d);
		pBuf->y = s_radius * MathUtil::sin(d);
		pBuf->z = 0;
		d += s_radialGranularity;

		*((unsigned int*)&pBuf->a) = GfxUtil::makePackedColor(s_zColour);
		++pBuf;
	}

	pVD->updateData();

	// repeat, but make a disc of a trifan
	/*
		X-AXIS ROTATION DISC
	*/

	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		362,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(TRIFAN);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtlTrans);
	pBuf = m_pVertData[GIZMO_ROTATION][3] = (Vert*)pVD->getBuffer();

	// draw disc around the X axis (in the Y and Z axes)
	unsigned int col = GfxUtil::makePackedColor(s_xRotateColour);
	pBuf->x = 0;
	pBuf->y = 0;
	pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;

	d = 0;
	for (size_t i=1; i<362; ++i)
	{
		pBuf->x = 0;
		pBuf->y = s_radius * MathUtil::cos(d);
		pBuf->z = s_radius * MathUtil::sin(d);
		d += s_radialGranularity;

		*((unsigned int*)&pBuf->a) = col;
		++pBuf;
	}

	pVD->updateData();

	/*
		Y-AXIS ROTATION DISC
	*/

	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		362,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(TRIFAN);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtlTrans);
	pBuf = m_pVertData[GIZMO_ROTATION][4] = (Vert*)pVD->getBuffer();

	// draw disc around the X axis (in the Y and Z axes)
	col = GfxUtil::makePackedColor(s_yRotateColour);
	pBuf->x = 0;
	pBuf->y = 0;
	pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;

	d = 0;
	for (size_t i=1; i<362; ++i)
	{
		pBuf->x = s_radius * MathUtil::sin(d);
		pBuf->y = 0;
		pBuf->z = s_radius * MathUtil::cos(d);
		d += s_radialGranularity;

		*((unsigned int*)&pBuf->a) = col;
		++pBuf;
	}

	pVD->updateData();

	/*
		Z-AXIS ROTATION DISC
	*/

	pSM = pGizmo->createSubMesh();
	pVD = pSM->createVertexData(
		strm,
		Environment::get(),
		sizeof(Vert),
		362,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(TRIFAN);
	pSM->setVertexFormat(
		Environment::get(),
		fmt);
	pSM->setMaterial(m_pMtlTrans);
	pBuf = m_pVertData[GIZMO_ROTATION][5] = (Vert*)pVD->getBuffer();

	// draw disc around the X axis (in the Y and Z axes)
	col = GfxUtil::makePackedColor(s_zRotateColour);
	pBuf->x = 0;
	pBuf->y = 0;
	pBuf->z = 0;
	*((unsigned int*)&pBuf->a) = col;
	++pBuf;

	d = 0;
	for (size_t i=1; i<362; ++i)
	{
		pBuf->x = s_radius * MathUtil::cos(d);
		pBuf->y = s_radius * MathUtil::sin(d);
		pBuf->z = 0;
		d += s_radialGranularity;

		*((unsigned int*)&pBuf->a) = col;
		++pBuf;
	}

	pVD->updateData();

	// make am instance of this mesh
	m_inst[0][GIZMO_ROTATION].setMeshHandle(m_gizmo[GIZMO_ROTATION]);
	m_inst[1][GIZMO_ROTATION].setMeshHandle(m_gizmo[GIZMO_ROTATION]);

	return true;
}
//---------------------------------------------------------------------------
bool TransformGizmo::createScaleGizmo(GfxVertexFormat& /*fmt*/)
{
#if 0 // use the position handles for scaling for now
	Environment& env = Environment::get();
	ResourceManager* pResMgr = env.pResourceMgr;

	/*
		Rotation Gizmo
	*/

	GfxMesh* pGizmo = AutoResPtr<GfxMesh>(m_gizmo[GIZMO_SCALE]);

	/*
		X-AXIS ROTATION CIRCLE
	*/

	GfxSubMesh* pSM = pGizmo->createSubMesh();
	size_t strm;
	GfxVertexData* pVD = pSM->createVertexData(
		strm,
		sizeof(Vert),
		362,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY));
	pSM->setPrimitiveType(LINESTRIP);
	pSM->setVertexFormat(fmt);
	pSM->setMaterial(m_pMtl);
	Vert* pBuf = (Vert*)pVD->lock();


	pVD->unlock();
#endif
	// make am instance of this mesh
	m_inst[0][GIZMO_SCALE].setMeshHandle(m_gizmo[GIZMO_POSITION]);
	m_inst[1][GIZMO_SCALE].setMeshHandle(m_gizmo[GIZMO_POSITION]);

#if 0

	// and finally the scaling quads
	pObj->begin("GizmoXColour", Ogre::RenderOperation::OT_LINE_STRIP);
	pObj->position(Ogre::Vector3(0, s_scaleHeight, 0));
	pObj->colour(s_xColour);
	pObj->position(Ogre::Vector3(0, s_scaleHeight, s_scaleHeight));
	pObj->colour(s_xColour);
	pObj->position(Ogre::Vector3(0, 0, s_scaleHeight));
	pObj->colour(s_xColour);
	pObj->end();

	// make an actual quad (2 tris)
	pObj->begin("GizmoXScaleColour", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
	pObj->position(0, s_scaleHeight, 0);
	pObj->colour(s_xScaleColour);
	pObj->position(Ogre::Vector3::ZERO);
	pObj->colour(s_xScaleColour);
	pObj->position(0, s_scaleHeight, s_scaleHeight);
	pObj->colour(s_xScaleColour);
	pObj->position(0, 0, s_scaleHeight);
	pObj->colour(s_xScaleColour);
	m_pRenderable[6] = pObj->end();

	pObj->begin("GizmoYColour", Ogre::RenderOperation::OT_LINE_STRIP);
	pObj->position(Ogre::Vector3(0, 0, s_scaleHeight));
	pObj->colour(s_yColour);
	pObj->position(Ogre::Vector3(s_scaleHeight, 0, s_scaleHeight));
	pObj->colour(s_yColour);
	pObj->position(Ogre::Vector3(s_scaleHeight, 0, 0));
	pObj->colour(s_yColour);
	pObj->end();

	// make an actual quad (2 tris)
	pObj->begin("GizmoYScaleColour", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
	pObj->position(s_scaleHeight, 0, 0);
	pObj->colour(s_yScaleColour);
	pObj->position(Ogre::Vector3::ZERO);
	pObj->colour(s_yScaleColour);
	pObj->position(s_scaleHeight, 0, s_scaleHeight);
	pObj->colour(s_yScaleColour);
	pObj->position(0, 0, s_scaleHeight);
	pObj->colour(s_yScaleColour);
	m_pRenderable[7] = pObj->end();

	pObj->begin("GizmoZColour", Ogre::RenderOperation::OT_LINE_STRIP);
	pObj->position(Ogre::Vector3(s_scaleHeight, 0, 0));
	pObj->colour(s_zColour);
	pObj->position(Ogre::Vector3(s_scaleHeight, s_scaleHeight, 0));
	pObj->colour(s_zColour);
	pObj->position(Ogre::Vector3(0, s_scaleHeight, 0));
	pObj->colour(s_zColour);
	pObj->end();

	// make an actual quad (2 tris)
	pObj->begin("GizmoZScaleColour", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
	pObj->position(s_scaleHeight, 0, 0);
	pObj->colour(s_zScaleColour);
	pObj->position(Ogre::Vector3::ZERO);
	pObj->colour(s_zScaleColour);
	pObj->position(s_scaleHeight, s_scaleHeight, 0);
	pObj->colour(s_zScaleColour);
	pObj->position(0, s_scaleHeight, 0);
	pObj->colour(s_zScaleColour);
	m_pRenderable[8] = pObj->end();

	static_cast<Ogre::SceneNode*>(m_pNode)->attachObject(pObj);
	m_pObj = pObj;

	// save off the vertex data so that we don't have to go to the GPU to 
	// get it every time we want to do a raycast (which is very frequent)
	for (int i=0; i<9; ++i)
	{
		Ogre::Renderable* pRend = m_pRenderable[i];
		Ogre::RenderOperation op;
		pRend->getRenderOperation(op);

		const Ogre::VertexElement* pElem = 
			op.vertexData->vertexDeclaration->findElementBySemantic(
					Ogre::VES_POSITION);

		const Ogre::HardwareVertexBufferSharedPtr& vbuf =
			op.vertexData->vertexBufferBinding->getBuffer(pElem->getSource());

		size_t stride = vbuf->getVertexSize();
		size_t size = vbuf->getSizeInBytes();

		// create float buffer to hold as many positions as exist in this buffer
		m_vertData[i].numVerts = vbuf->getNumVertices();
		m_vertData[i].pPosData = new Ogre::Vector3[m_vertData[i].numVerts];
		m_vertData[i].bTrifan = (op.operationType == Ogre::RenderOperation::OT_TRIANGLE_FAN);
		char* pData = (char*)vbuf->lock(Ogre::HardwareBuffer::HBL_NORMAL);

		for (size_t j=0, k=0; j<size; j+=stride, ++k)
		{
			float* pf = (float*)&pData[j];
			m_vertData[i].pPosData[k].x = *pf;
			m_vertData[i].pPosData[k].y = *(pf+1);
			m_vertData[i].pPosData[k].z = *(pf+2);
		}

		vbuf->unlock();
	}
#endif

	return true;
}
//---------------------------------------------------------------------------
void TransformGizmo::createGizmo()
{
	if (isInitialized())
		return;

	Environment& env = Environment::get();
	ResourceManager* pResMgr = env.pResourceMgr;

	if (m_gizmo[0] == INVALID_RESOURCE_HANDLE)
		m_gizmo[0] = pResMgr->acquire(GfxMesh::RESOURCE_TYPE);
	if (m_gizmo[1] == INVALID_RESOURCE_HANDLE)
		m_gizmo[1] = pResMgr->acquire(GfxMesh::RESOURCE_TYPE);
	if (m_gizmo[2] == INVALID_RESOURCE_HANDLE)
		m_gizmo[2] = pResMgr->acquire(GfxMesh::RESOURCE_TYPE);

	// common object creation (vert format, materials)
	GfxVertexFormat fmt;
	GfxVertexFormat::Element elem;
	elem.semantic = POSITION;
	elem.offset = 0;
	elem.setSource(0);
	elem.setUsage(0);
	elem.type = FLOAT3;
	fmt.addElement(elem);
	elem.offset = (unsigned char)GfxUtil::getSizeOf((VertexElementType)elem.type);
	elem.type = VET_COLOR;
	elem.semantic = COLOR;
	fmt.addElement(elem);

	// shared material that all submeshes will use (since everything is vertex-colored)
	m_pMtl = COS_NEW GfxMaterial;
	m_pMtl->setDiffuse(GfxUtil::packColor(128, 128, 128, 255));
	m_pMtl->setCullMode(GfxMaterial::CULL_NONE);

	m_pMtlSelected = COS_NEW GfxMaterial;
	m_pMtlSelected->setDiffuse(GfxUtil::packColor(255, 255, 255, 255));
	m_pMtlSelected->setCullMode(GfxMaterial::CULL_NONE);

	m_pMtlTrans = COS_NEW GfxMaterial;
	m_pMtlTrans->setDiffuse(GfxUtil::packColor(128, 128, 128, 128));
	m_pMtlTrans->setDepthWrite(false);
	m_pMtlTrans->setDepthCheck(false);
	m_pMtlTrans->setAlphaBlended(true);
	m_pMtlTrans->setCullMode(GfxMaterial::CULL_NONE);

	m_pMtlTransSelected = COS_NEW GfxMaterial;
	m_pMtlTransSelected->setDiffuse(GfxUtil::packColor(255, 255, 255, 128));
	m_pMtlTransSelected->setDepthWrite(false);
	m_pMtlTransSelected->setDepthCheck(false);
	m_pMtlTransSelected->setAlphaBlended(true);
	m_pMtlTransSelected->setCullMode(GfxMaterial::CULL_NONE);

	createPositionGizmo(fmt);
	createRotationGizmo(fmt);
	createScaleGizmo(fmt);

	m_inst[0][GIZMO_POSITION].createMaterialOverrides(6);
	m_inst[0][GIZMO_ROTATION].createMaterialOverrides(6);
	m_inst[0][GIZMO_SCALE].createMaterialOverrides(6);
	m_inst[1][GIZMO_POSITION].createMaterialOverrides(6);
	m_inst[1][GIZMO_ROTATION].createMaterialOverrides(6);
	m_inst[1][GIZMO_SCALE].createMaterialOverrides(6);
}
//---------------------------------------------------------------------------
bool TransformGizmo::initialize()
{
	createGizmo();
	return true;
}
//---------------------------------------------------------------------------
bool TransformGizmo::destroy()
{
	for (int i=0; i<2; ++i)
		for (int j=0; j<3; ++j)
			m_inst[i][j].destroy();

	if (m_pMtl)
	{
		m_pMtl->destroy();
		delete m_pMtl;
		m_pMtl = 0;
	}

	if (m_pMtlTrans)
	{
		m_pMtlTrans->destroy();
		delete m_pMtlTrans;
		m_pMtlTrans = 0;
	}

	if (m_pMtlSelected)
	{
		m_pMtlSelected->destroy();
		delete m_pMtlSelected;
		m_pMtlSelected = 0;
	}

	if (m_pMtlTransSelected)
	{
		m_pMtlTransSelected->destroy();
		delete m_pMtlTransSelected;
		m_pMtlTransSelected = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
void TransformGizmo::show()
{
	m_bVisible = true;
}
//---------------------------------------------------------------------------
void TransformGizmo::hide()
{
	m_bVisible = false;
}
//---------------------------------------------------------------------------
void TransformGizmo::update(float /*deltaSeconds*/)
{
	if (m_pObj)
	{
		float objRad = m_pObj->getRadius();

		// some objects that are manipulable, have no radius -- for those,
		// assume a radius of 5 (just to make the gizmo easy to use/see)
		if (objRad == 0)
			objRad = 5;

		float scaleFactor = objRad / s_radius * 1.2f;
		Transform xform(m_pObj->getTransform());
		xform.scale = Vector4::UNIT_SCALE * scaleFactor; // don't inherit scaling
		m_inst[0][m_currentGizmo].setTransform(xform);
		m_inst[1][m_currentGizmo].setTransform(xform);
	}
}
//---------------------------------------------------------------------------
void TransformGizmo::freeze()
{
	m_bFreezeOperation = true;
}
//---------------------------------------------------------------------------
void TransformGizmo::unfreeze()
{
	m_bFreezeOperation = false;
}
//---------------------------------------------------------------------------
void TransformGizmo::render()
{
}
//---------------------------------------------------------------------------
bool TransformGizmo::isVisible()
{
	return m_bVisible;
}
//---------------------------------------------------------------------------
void TransformGizmo::setObject(ZoneObject* pObj)
{
	m_pObj = pObj;

	if (!m_pObj)
		hide();
	else
		show();
}
//---------------------------------------------------------------------------
ZoneObject* TransformGizmo::getObject()
{
	return m_pObj;
}
//---------------------------------------------------------------------------
int TransformGizmo::getOperations()
{
	return m_operations;
}
//---------------------------------------------------------------------------
bool TransformGizmo::castRay(const Ray& ray, Vector4& intersectPoint)
{
	if (m_bFreezeOperation || !m_bVisible || !m_pObj)
		return false;

	// go through the vert buffers for the selected gizmo
	Vector4 pos[3];
	Transform xform(m_inst[0][m_currentGizmo].getTransform());
	const Quaternion& q = xform.rot;
	const Vector4& p = xform.trans;
	const Vector4& s = xform.scale;
	m_operations = OP_NONE; // this value is rebuilt every raycast

	// we only need to test against the upper three buffers; the lower three
	// are the "outline" buffers and testing against them is a waste
	GfxMesh* pMesh = AutoResPtr<GfxMesh>(m_gizmo[m_currentGizmo]);
	float distToIntersect = MathUtil::POSITIVE_INFINITY;
	int nearestTri = 0;

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

	highlightOperations();
	return rtn;
}
//---------------------------------------------------------------------------
GfxMeshInstance& TransformGizmo::getMeshInstance(size_t passIndex)
{
	return m_inst[passIndex][m_currentGizmo];
}
//---------------------------------------------------------------------------
void TransformGizmo::highlightOperations()
{
	int ops = (m_operations >> m_currentGizmo*3);

	// clear any selection material(s)
	m_inst[0][m_currentGizmo].setMaterialByIndex(0, m_pMtl);
	m_inst[0][m_currentGizmo].setMaterialByIndex(1, m_pMtl);
	m_inst[0][m_currentGizmo].setMaterialByIndex(2, m_pMtl);
	m_inst[0][m_currentGizmo].setMaterialByIndex(3, m_pMtl);
	m_inst[0][m_currentGizmo].setMaterialByIndex(4, m_pMtl);
	m_inst[0][m_currentGizmo].setMaterialByIndex(5, m_pMtl);
	m_inst[1][m_currentGizmo].setMaterialByIndex(0, m_pMtlTrans);
	m_inst[1][m_currentGizmo].setMaterialByIndex(1, m_pMtlTrans);
	m_inst[1][m_currentGizmo].setMaterialByIndex(2, m_pMtlTrans);
	m_inst[1][m_currentGizmo].setMaterialByIndex(3, m_pMtlTrans);
	m_inst[1][m_currentGizmo].setMaterialByIndex(4, m_pMtlTrans);
	m_inst[1][m_currentGizmo].setMaterialByIndex(5, m_pMtlTrans);

	if (ops & 1) // red/X
	{
		m_inst[0][m_currentGizmo].setMaterialByIndex(0, m_pMtlSelected);
		m_inst[0][m_currentGizmo].setMaterialByIndex(3, m_pMtlSelected);
		m_inst[1][m_currentGizmo].setMaterialByIndex(0, m_pMtlTransSelected);
		m_inst[1][m_currentGizmo].setMaterialByIndex(3, m_pMtlTransSelected);
	}

	if (ops & 2) // green/Y
	{
		m_inst[0][m_currentGizmo].setMaterialByIndex(1, m_pMtlSelected);
		m_inst[0][m_currentGizmo].setMaterialByIndex(4, m_pMtlSelected);
		m_inst[1][m_currentGizmo].setMaterialByIndex(1, m_pMtlTransSelected);
		m_inst[1][m_currentGizmo].setMaterialByIndex(4, m_pMtlTransSelected);
	}

	if (ops & 4) // blue/Z
	{
		m_inst[0][m_currentGizmo].setMaterialByIndex(2, m_pMtlSelected);
		m_inst[0][m_currentGizmo].setMaterialByIndex(5, m_pMtlSelected);
		m_inst[1][m_currentGizmo].setMaterialByIndex(2, m_pMtlTransSelected);
		m_inst[1][m_currentGizmo].setMaterialByIndex(5, m_pMtlTransSelected);
	}
}
//---------------------------------------------------------------------------
TransformGizmo::ActiveGizmo TransformGizmo::getGizmo() const
{
	return m_currentGizmo;
}
//---------------------------------------------------------------------------
void TransformGizmo::setGizmo(ActiveGizmo gizmo)
{
	m_currentGizmo = gizmo;
}
