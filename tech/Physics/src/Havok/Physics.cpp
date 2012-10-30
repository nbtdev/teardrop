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
#include "Physics.h"
#include "PhysicsMemory.h"
#include "WorldHavok.h"
#include "BodyHavok.h"
#include "HeightfieldShapeHavok.h"
#include "Capsule.h"
#include "CharacterProxyHavok.h"
#include "Util/include/Environment.h"
#include "Util/include/Logger.h"
#include "Math/include/MathUtil.h"
#include "Memory/include/Memory.h"
#include <iostream>

using namespace CoS;
//---------------------------------------------------------------------------
Allocator* Physics::m_pAllocator = 0; // USER MUST SET THIS
static const int PHYSICS_STACK_SIZE = 4 * 1024 * 1024;
static char s_stack[PHYSICS_STACK_SIZE];

// for Havok visual debugger
static hkpPhysicsContext* s_pPhysicsContext = 0;
static hkVisualDebugger* s_pDebugger = 0;
//---------------------------------------------------------------------------
static void errorFn(const char* pMessage, void* pObj)
{
	Environment::get().pLogger->logMessage(pMessage);
}//---------------------------------------------------------------------------
static void errorFnStdOut(const char* pMessage, void* pObj)
{
	std::cout << pMessage << std::endl;
}
//---------------------------------------------------------------------------
void Physics::setAllocator(Allocator* pAlloc)
{
	m_pAllocator = pAlloc;
}
//---------------------------------------------------------------------------
Allocator* Physics::getAllocator()
{
	return m_pAllocator;
}
//---------------------------------------------------------------------------
bool Physics::initialize(bool bEnableLogging, bool bEnableVDB)
{
	// static-init hackery
	BodyHavok cBodyHavok;
	cBodyHavok.destroy();

	PhysicsMemory* s_physAlloc = new PhysicsMemory;
	hkMemoryRouter* memRouter = hkMemoryInitUtil::initDefault(s_physAlloc);

	void (*errFn)(const char*, void*) = bEnableLogging ? errorFn : errorFnStdOut;
	if (HK_SUCCESS != hkBaseSystem::init(memRouter, errFn))
		return false;

	hkSerializeDeprecated::initDeprecated();

	if (bEnableVDB)
	{
		// fire up the visual debug server
		s_pPhysicsContext = new hkpPhysicsContext;
		s_pPhysicsContext->registerAllPhysicsProcesses();

		hkArray<hkProcessContext*> contexts;
		contexts.pushBack(s_pPhysicsContext);
		s_pDebugger = new hkVisualDebugger(contexts);
		s_pDebugger->serve();

	}

	return true;
}
//---------------------------------------------------------------------------
bool Physics::shutdown()
{
	if (s_pDebugger)
	{
		s_pDebugger->shutdown();
		s_pDebugger->removeReference();
		s_pPhysicsContext->removeReference();
		s_pDebugger = 0;
		s_pPhysicsContext = 0;
	}

	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();

	return true;
}
//---------------------------------------------------------------------------
void Physics::addWorldToDebugger(World* pWorld)
{
	if (s_pDebugger)
	{
		WorldHavok* pHWorld = static_cast<WorldHavok*>(pWorld);
		s_pPhysicsContext->addWorld(pHWorld->getHavokWorld());
	}
}
//---------------------------------------------------------------------------
void Physics::removeWorldFromDebugger(World* pWorld)
{
	if (s_pDebugger)
	{
		WorldHavok* pHWorld = static_cast<WorldHavok*>(pWorld);
		s_pPhysicsContext->removeWorld(pHWorld->getHavokWorld());
	}
}
//---------------------------------------------------------------------------
void Physics::advanceDebugger(float deltaT)
{
	if (s_pDebugger)
		s_pDebugger->step(deltaT * 1000);
}
//---------------------------------------------------------------------------
void Physics::destroyShape(Shape* pShape)
{
	if (pShape)
		pShape->release();

	delete pShape;
}
//---------------------------------------------------------------------------
Shape* Physics::createHeightfieldShape(
	void* pData,
	size_t resX,
	size_t resZ,
	size_t bytesPerSample,
	const Vector4& scale)
{
	// we need to hide this because Havok wants this info in the c'tor
	hkpSampledHeightFieldBaseCinfo ci;
	ci.m_xRes = (int)resX;
	ci.m_zRes = (int)resZ;
	
	hkVector4 _scale(
		scale.x / (float)resX,
		scale.y / 255,
		scale.z / (float)resZ,
		0);
	ci.m_scale = _scale;
	HeightfieldShapeHavok* pRtn = COS_NEW HeightfieldShapeHavok(ci);
	pRtn->setUserData((hkUlong)pData);
	if (!pRtn->initialize())
	{
		delete pRtn;
		return 0;
	}

	pRtn->setUserData(0);
	return pRtn;
}
//---------------------------------------------------------------------------
World* Physics::createWorld(const AABB& aabb, float cdTolerance)
{
	// we need to hide this because Havok wants this info in the c'tor
	hkpWorldCinfo ci;
	ci.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_ASSERT;
	Vector4 ext2(aabb.extents * 1.1f);
	float worldSize = MathUtil::max(ext2.x, MathUtil::max(ext2.y, ext2.z));
	ci.setBroadPhaseWorldSize(worldSize);
	ci.m_collisionTolerance = cdTolerance;
	WorldHavok* pRtn = COS_NEW WorldHavok(ci);
	if (!pRtn->initialize(aabb))
	{
		return 0;
	}

	return pRtn;
}
//---------------------------------------------------------------------------
Body* Physics::createBody()
{
	return COS_NEW BodyHavok;
}
//---------------------------------------------------------------------------
void Physics::destroyBody(Body* pBody)
{
	if (pBody)
		pBody->release();

	delete pBody;
}
//---------------------------------------------------------------------------
Shape* Physics::createCapsule(
	const Vector4& p0,
	const Vector4& p1,
	float radius)
{
	Capsule* pCapsule = COS_NEW Capsule;
	pCapsule->initialize(p0, p1, radius);
	return pCapsule;
}
//---------------------------------------------------------------------------
void Physics::destroyCharacterProxy(CharacterProxy* pProxy)
{
	if (pProxy)
		pProxy->release();

	delete pProxy;
}
//---------------------------------------------------------------------------
CharacterProxy* Physics::createCharacterProxy(
	Shape* pShape,
	const Vector4& worldPos,
	float mass,
	float maxSlopeInRadians,
	size_t materialId)
{
	CharacterProxy* p = COS_NEW CharacterProxyHavok;
	p->initialize(
		pShape,
		worldPos,
		mass,
		maxSlopeInRadians,
		0.2f,
		materialId);

	return p;
}
