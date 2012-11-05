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
#include "BodyHavok.h"
#include "ShapeHavok.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Math/Transform.h"
#include "Math/Vector4.h"
#include "Memory/Memory.h"
#include "Serialization/ResourceSerializer.h"

using namespace CoS;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(BodyHavok);
//---------------------------------------------------------------------------
hkpMotion::MotionType s_motionTypeLut[] = 
{
	/*
		enum MotionType
		{
			STATIC,
			DYNAMIC,
			KEYFRAMED
		};
	*/
	hkpMotion::MOTION_FIXED,
	hkpMotion::MOTION_DYNAMIC,
	hkpMotion::MOTION_KEYFRAMED,
};
//---------------------------------------------------------------------------
BodyHavok::BodyHavok()
{
	m_pBody = 0;
}
//---------------------------------------------------------------------------
BodyHavok::BodyHavok(int i)
{
	UNREFERENCED_PARAMETER(i);
	m_pBody = 0;
}
//---------------------------------------------------------------------------
BodyHavok::~BodyHavok()
{
}
//---------------------------------------------------------------------------
bool BodyHavok::initialize(
	void* pData,
	unsigned int dataLen
	)
{
	return Body::initialize(pData, dataLen);
}
//---------------------------------------------------------------------------
bool BodyHavok::initialize(
	Shape* pShape, 
	MotionType motionType,
	const Vector4& origin, 
	const Vector4& inertia)
{
	release();

	ShapeHavok* pHShape = static_cast<ShapeHavok*>(pShape);
	m_pShape = pShape;
	hkpRigidBodyCinfo ci;
	ci.m_motionType = s_motionTypeLut[motionType];
	ci.m_position.set(origin.x, origin.y, origin.z);
	ci.m_shape = pHShape->getHavokShape();
	ci.m_friction = 0.2f;
	m_pBody = new hkpRigidBody(ci);
	return Body::initialize(pShape, motionType, origin, inertia);
}
//---------------------------------------------------------------------------
bool BodyHavok::release()
{
	if (m_pBody)
	{
		m_pBody->removeReference();
		m_pBody = 0;
	}

	if (m_pResource)
	{
		m_pResource->removeReference();
		m_pResource = 0;
	}

	return Body::release();
}
//---------------------------------------------------------------------------
void BodyHavok::delayInit()
{
	hkSerializeUtil::ErrorDetails err;
	// check to see if we need to try to init
	if (!m_pBody)
	{
		m_pResource = hkSerializeUtil::load(m_pData, m_dataLen, &err);
		if (!m_pResource)
		{
			Environment::get().pLogger->logMessage(err.defaultMessage);
			return;
		}
		hkRootLevelContainer* pRC = m_pResource->getContents<hkRootLevelContainer>();
		hkpPhysicsData* pData = static_cast<hkpPhysicsData*>(
			pRC->findObjectByType(hkpPhysicsDataClass.getName()));
		hkArray<hkpPhysicsSystem*> pSys;
		pSys = pData->getPhysicsSystems();

		hkArray<hkpRigidBody*> bodies;
		bodies = pSys[0]->getRigidBodies();

		if (bodies.getSize())
		{
			m_pBody = static_cast<hkpRigidBody*>(bodies[0]->clone());
		}
	}
}
//---------------------------------------------------------------------------
void* BodyHavok::getUserData()
{
	if (m_pBody)
		return (void*)m_pBody->getUserData();

	return 0;
}
//---------------------------------------------------------------------------
void BodyHavok::setUserData(void* pData)
{
	if (m_pBody)
		m_pBody->setUserData(hkUlong(pData));
}
//---------------------------------------------------------------------------
Body* BodyHavok::clone()
{
	return 0;
}
//---------------------------------------------------------------------------
bool BodyHavok::update(float deltaT)
{
	delayInit();
	return Body::update(deltaT);
}
//---------------------------------------------------------------------------
hkpRigidBody* BodyHavok::getHavokBody()
{
	delayInit();
	return m_pBody;
}
//---------------------------------------------------------------------------
void BodyHavok::changeMotionType(MotionType type)
{
	delayInit();
	m_pBody->setMotionType(s_motionTypeLut[type]);
}
//---------------------------------------------------------------------------
void BodyHavok::getLinearVelocity(Vector4& vel)
{
	delayInit();
}
//---------------------------------------------------------------------------
void BodyHavok::getAngularVelocity(Vector4& vel)
{
	delayInit();
}
//---------------------------------------------------------------------------
void BodyHavok::getWorldTransform(Transform& xform)
{
	delayInit();
}
//---------------------------------------------------------------------------
void BodyHavok::setLinearVelocity(const Vector4& vel)
{
	delayInit();
	m_pBody->setLinearVelocity((hkVector4&)vel);
}
//---------------------------------------------------------------------------
void BodyHavok::setAngularVelocity(const Vector4& vel)
{
	delayInit();
	m_pBody->setAngularVelocity((hkVector4&)vel);
}
//---------------------------------------------------------------------------
void BodyHavok::setWorldTransform(const Transform& xform)
{
	delayInit();
	m_pBody->setPositionAndRotation(
		(hkVector4&)xform.trans,
		(hkQuaternion&)xform.rot);
}
//---------------------------------------------------------------------------
bool BodyHavok::getDisplayGeometry(DisplayGeometries& geom)
{
	if (!m_pBody)
		return false;

	hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment env;
	hkpShapeDisplayBuilder builder(env);

	hkArray<hkDisplayGeometry*> displayGeom;
	//builder.buildShapeDisplay(m_pBody->getCollidable()->getShape(), m_pBody->getTransform(), displayGeom);
	builder.buildShapeDisplay(m_pBody->getCollidable()->getShape(), hkTransform::getIdentity(), displayGeom);

	for (int i=0; i<displayGeom.getSize(); ++i)
	{
		hkArray<hkVector4> lines;
		displayGeom[i]->getWireframeGeometry(lines);

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
//---------------------------------------------------------------------------
bool BodyHavok::serialize(ResourceSerializer& ser)
{
	ser.start();

	// serialize the vtabl pointer entry
	ser.relocate(BodyHavok::getSerialClass()->getId());

	// before we write the memory image, we want to make sure that the
	// "non-serializable" members are nulled (and then restored)
	hkpRigidBody* pBody = m_pBody;
	m_pBody = 0;
	Shape* pShape = m_pShape;
	m_pShape = 0;
	void* pUserData = m_pUserData;
	pUserData = 0;

	ser.relocate(this, sizeof(BodyHavok));

	m_pBody = pBody;
	m_pShape = pShape;
	m_pUserData = pUserData;

	// relocate the data serial pointer
	ser.relocate(m_pData);

	// and then relocate the actual data
	size_t sz = m_dataLen;
	ser.relocate(m_pData, sz);

	return ser.finalize();
}
