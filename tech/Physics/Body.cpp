/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Physics.h"
#include "Body.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
//#include "Util/SystemManager.h"
//#include "Util/System.h"
#include "Memory/Allocators.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Body::Body()
{
	m_pShape = 0;
	m_pData = 0;
	m_dataLen = 0;
}
//---------------------------------------------------------------------------
Body::Body(int /*i*/)
{
	m_pShape = 0;
}
//---------------------------------------------------------------------------
Body::~Body()
{
}
//---------------------------------------------------------------------------
bool Body::initialize(
	void* pData,
	unsigned int dataLen
	)
{
	m_pData = pData;
	m_dataLen = dataLen;
	return true;
}
//---------------------------------------------------------------------------
bool Body::initialize(
	Shape* pShape, 
	MotionType /*motionType*/,
	const Vector4& /*origin*/, 
	const Vector4& /*inertia*/)
{
	m_pShape = pShape;
	return true;
}
//---------------------------------------------------------------------------
bool Body::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool Body::release()
{
	return true;
}
//---------------------------------------------------------------------------
bool Body::update(float /*deltaT*/)
{
	return true;
}
//---------------------------------------------------------------------------
Shape* Body::getShape()
{
	return m_pShape;
}
//---------------------------------------------------------------------------
Body* Body::clone()
{
	// revisit this when we have a solution for cloning havok rigid bodies
	return 0;
}
//---------------------------------------------------------------------------
void Body::changeMotionType(MotionType /*type*/)
{
}
//---------------------------------------------------------------------------
void Body::getLinearVelocity(Vector4& /*vel*/)
{
}
//---------------------------------------------------------------------------
void Body::getAngularVelocity(Vector4& /*vel*/)
{
}
//---------------------------------------------------------------------------
void Body::getWorldTransform(Transform& /*xform*/)
{
}
//---------------------------------------------------------------------------
void Body::setLinearVelocity(const Vector4& /*vel*/)
{
}
//---------------------------------------------------------------------------
void Body::setAngularVelocity(const Vector4& /*vel*/)
{
}
//---------------------------------------------------------------------------
void Body::setWorldTransform(const Transform& /*xform*/)
{
}
////---------------------------------------------------------------------------
//bool Body::load(Stream& strm)
//{
//	Teardrop::System* pSys =
//		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_PHYSICS);

//	// load the whole stream and own the data once loaded
//	if (m_pData)
//	{
//		pSys->getAllocator()->Deallocate(m_pData);
//	}

//	unsigned int len = (unsigned int)strm.length();
//	m_pData = pSys->getAllocator()->AllocateAligned(len, 16 TD_ALLOC_SITE);
//	strm.read(m_pData, len);

//	return initialize(m_pData, len);
//}
////---------------------------------------------------------------------------
//bool Body::serialize(ResourceSerializer& /*ser*/)
//{
//	return false;
//}
//---------------------------------------------------------------------------
void* Body::getUserData()
{
	return 0;
}
//---------------------------------------------------------------------------
void Body::setUserData(void* /*pData*/)
{
}
//---------------------------------------------------------------------------
bool Body::getDisplayGeometry(DisplayGeometries& /*geom*/)
{
	// implemented by derived classes
	return false;
}
