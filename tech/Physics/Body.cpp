/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
