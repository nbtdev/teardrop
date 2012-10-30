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
#include "CharacterProxyHavok.h"
#include "ShapeHavok.h"
#include "WorldHavok.h"
#include "CollisionFilter.h"
#include "Math/include/Transform.h"
#include "Math/include/Quaternion.h"
#include "Math/include/Vector4.h"
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Dynamics/Phantom/hkpCachingShapePhantom.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>

using namespace CoS;
//---------------------------------------------------------------------------
CharacterProxyHavok::CharacterProxyHavok()
{
	m_pProxy = 0;
	m_pPhantom = 0;
	m_pWorld = 0;
	m_materialId = size_t(-1);
	m_pUserData = 0;
}
//---------------------------------------------------------------------------
CharacterProxyHavok::~CharacterProxyHavok()
{
}
//---------------------------------------------------------------------------
bool CharacterProxyHavok::initialize(
	Shape* pShape,
	const Vector4& worldPos,
	float mass,
	float maxSlopeInRadians,
	float friction,
	size_t materialId)
{
	m_pPhantom = new hkpSimpleShapePhantom(
		static_cast<ShapeHavok*>(pShape)->getHavokShape(),
		hkTransform(hkQuaternion::getIdentity(), (const hkVector4&)worldPos),
		hkpGroupFilter::calcFilterInfo(COLLISION_LAYER_CHARACTER_PROXY, COLLISION_GROUP_CHARACTER));

	m_materialId = materialId;

	hkpCharacterProxyCinfo ci;
	ci.m_characterMass = mass;
	ci.m_dynamicFriction = friction;
	ci.m_maxSlope = maxSlopeInRadians;
	ci.m_position = (hkVector4&)worldPos;
	ci.m_shapePhantom = m_pPhantom;

	m_pProxy = new hkpCharacterProxy(ci);

	return true;
}
//---------------------------------------------------------------------------
bool CharacterProxyHavok::release()
{
	if (m_pWorld)
	{
		m_pWorld->removePhantom(m_pPhantom);
	}

	if (m_pProxy)
	{
		m_pProxy->removeReference();
	}

	if (m_pPhantom)
	{
		m_pPhantom->removeReference();
	}

	return true;
}
//---------------------------------------------------------------------------
bool CharacterProxyHavok::update(float deltaT)
{
	if (m_pWorld)
	{
		hkStepInfo stepInfo(0, deltaT);
		m_pProxy->integrate(stepInfo, m_pWorld->getGravity());
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::getWorldTransform(Transform& xform) const
{
	xform.trans = (Vector4&)m_pProxy->getPosition();
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::getLinearVelocity(Vector4& vel) const
{
	vel = (Vector4&)m_pProxy->getLinearVelocity();
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::getAngularVelocity(Vector4& vel) const
{
	vel = Vector4::ZERO;
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::setWorldTransform(const Transform& xform)
{
	m_pProxy->setPosition((hkVector4&)xform.trans);
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::setLinearVelocity(const Vector4& vel)
{
	m_pProxy->setLinearVelocity((hkVector4&)vel);
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::setAngularVelocity(const Vector4& vel)
{
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::addToWorld(World* pWorld)
{
	if (pWorld)
	{
		m_pWorld = static_cast<WorldHavok*>(pWorld)->getHavokWorld();
		m_pWorld->addPhantom(m_pPhantom);
	}
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::removeFromWorld(World* pWorld)
{
	if (pWorld)
	{
		m_pWorld = static_cast<WorldHavok*>(pWorld)->getHavokWorld();
		m_pWorld->removePhantom(m_pPhantom);
		m_pWorld = 0;
	}
}
//---------------------------------------------------------------------------
bool CharacterProxyHavok::isSupported() const
{
	if (m_pProxy)
	{
		hkpSurfaceInfo info;
		m_pProxy->checkSupport(hkVector4(0,-1,0), info);
		return (info.m_supportedState != hkpSurfaceInfo::UNSUPPORTED);
	}

	return true;
}
//---------------------------------------------------------------------------
void CharacterProxyHavok::setUserData(void* pData)
{
	if (m_pPhantom)
		m_pPhantom->setUserData(hkUlong(pData));
}
//---------------------------------------------------------------------------
void* CharacterProxyHavok::getUserData()
{
	if (m_pPhantom)
		return (void*)m_pPhantom->getUserData();

	return 0;
}
//---------------------------------------------------------------------------
bool CharacterProxyHavok::getDisplayGeometry(DisplayGeometries& geom)
{
	if (!m_pPhantom)
		return false;

	hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment env;
	hkpShapeDisplayBuilder builder(env);

	hkArray<hkDisplayGeometry*> displayGeom;
	//builder.buildShapeDisplay(m_pPhantom->getCollidable()->getShape(), m_pPhantom->getTransform(), displayGeom);
	builder.buildShapeDisplay(m_pPhantom->getCollidable()->getShape(), hkTransform::getIdentity(), displayGeom);

	for (int i=0; i<displayGeom.getSize(); ++i)
	{
		displayGeom[i]->buildGeometry();

		hkArray<hkVector4> lines;
		displayGeom[i]->getWireframeGeometry(lines);

		if (lines.getSize() == 0)
		{
			// then we have to build the wireframe geom ourselves; do it brute-force
			// and duplicate edges if needed

			hkGeometry* pGeom = displayGeom[i]->m_geometry;
			int numTris = pGeom->m_triangles.getSize();
			int numLines = numTris * 3 * 2;
			lines.setSize(numLines);

			int line = 0;
			for (int t=0; t<numTris; ++t)
			{
				hkGeometry::Triangle& tri = pGeom->m_triangles[t];
				lines[line++] = pGeom->m_vertices[tri.m_a];
				lines[line++] = pGeom->m_vertices[tri.m_b];
				lines[line++] = pGeom->m_vertices[tri.m_b];
				lines[line++] = pGeom->m_vertices[tri.m_c];
				lines[line++] = pGeom->m_vertices[tri.m_c];
				lines[line++] = pGeom->m_vertices[tri.m_a];
			}
		}

		DisplayGeometry dg;
		dg.reserve(lines.getSize());
		dg.resize(lines.getSize());
		geom.push_back(dg);

		hkVector4* pDst = (hkVector4*)&(geom.back()[0]);
		memcpy(pDst, lines.begin(), sizeof(hkVector4) * lines.getSize());

		// yay, thanks Havok -- now I get to go fix your damn .w=0 problem...
		for (int v=0; v<lines.getSize(); ++v)
		{
			hkVector4& vec = *(pDst + v);
			vec(3) = 1;
		}
	}

	return true;
}
