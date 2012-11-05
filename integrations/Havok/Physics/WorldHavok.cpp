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
#include "WorldHavok.h"
#include "BodyHavok.h"
#include "CollisionFilterHavok.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Math/Ray.h"
#include "Math/Vector4.h"
#include "Math/MathUtil.h"

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Physics/Dynamics/World/BroadPhaseBorder/hkpBroadPhaseBorder.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCaster.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
struct WorldBorderMonitor : public hkpBroadPhaseBorder
{
	WorldBorderMonitor(
		hkpWorld* world, hkpWorldCinfo::BroadPhaseBorderBehaviour behav)
		: hkpBroadPhaseBorder(world, behav)
	{
	}

	void maxPositionExceededCallback (hkpEntity *body) 
	{
	}
};
//---------------------------------------------------------------------------
WorldHavok::WorldHavok(hkpWorldCinfo& ci) : m_ci(ci)
{
	m_pWorld = 0;
	m_pMon = 0;
}
//---------------------------------------------------------------------------
WorldHavok::~WorldHavok()
{
}
//---------------------------------------------------------------------------
bool WorldHavok::initialize(const AABB& aabb)
{
	assert(!m_pWorld);
	if (m_pWorld)
	{
		m_pWorld->removeReference();
	}

	delete m_pMon;
	m_pMon = 0;

	// create thread pool and job queue
	//hkCpuJobThreadPoolCinfo ciThreadPool;
	//hkHardwareInfo hwInfo;
	//hkGetHardwareInfo(hwInfo);

	//ciThreadPool.m_numThreads = hwInfo.m_numThreads - 1;
	//ciThreadPool.m_timerBufferPerThreadAllocation = 2000000;
	//m_pThreadPool = new hkCpuJobThreadPool(ciThreadPool);

	//hkJobQueueCinfo ciQueue;
	//ciQueue.m_jobQueueHwSetup.m_numCpuThreads = hwInfo.m_numThreads;
	//m_pJobQueue = new hkJobQueue(ciQueue);

	// enable multithreaded step
	//m_ci.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;

	m_pWorld = new hkpWorld(m_ci);
	//m_pWorld->registerWithJobQueue(m_pJobQueue);

	//m_pWorld->markForWrite();
	m_pMon = new WorldBorderMonitor(m_pWorld, hkpWorldCinfo::BROADPHASE_BORDER_ASSERT);
	hkpAgentRegisterUtil::registerAllAgents( m_pWorld->getCollisionDispatcher() );
	//m_pWorld->unmarkForWrite();
	return true;
}
//---------------------------------------------------------------------------
bool WorldHavok::release()
{
	if (m_pWorld)
	{
		m_pWorld->removeReference();
		m_pWorld = 0;
	}

	//delete m_pJobQueue;
	//m_pThreadPool->removeReference();

	delete m_pMon;
	m_pMon = 0;

	return World::release();
}
//---------------------------------------------------------------------------
hkpWorld* WorldHavok::getHavokWorld()
{
	return m_pWorld;
}
//---------------------------------------------------------------------------
bool WorldHavok::update(float deltaT)
{
	if (m_pWorld && deltaT > MathUtil::EPSILON)
	{
		/*hkpStepResult res =*/ m_pWorld->stepDeltaTime(deltaT);
		//hkpStepResult res = 
		//	m_pWorld->stepMultithreaded(m_pJobQueue, m_pThreadPool, deltaT);
		//return (res == HK_STEP_RESULT_SUCCESS);
	}

	return true;
}
//---------------------------------------------------------------------------
void WorldHavok::clear()
{
	if (m_pWorld)
	{
		m_pWorld->removeAll();
	}
}
//---------------------------------------------------------------------------
bool WorldHavok::add(Body* pBody)
{
	BodyHavok* pHBody = static_cast<BodyHavok*>(pBody);
	if (m_pWorld)
	{
		m_pWorld->addEntity(pHBody->getHavokBody());
	}

	return true;
}
//---------------------------------------------------------------------------
bool WorldHavok::add(Phantom* pPhantom)
{
	if (m_pWorld)
	{
	}

	return true;
}
//---------------------------------------------------------------------------
bool WorldHavok::remove(Body* pBody)
{
	BodyHavok* pHBody = static_cast<BodyHavok*>(pBody);
	if (m_pWorld)
	{
		m_pWorld->removeEntity(pHBody->getHavokBody());
	}

	return true;
}
//---------------------------------------------------------------------------
bool WorldHavok::remove(Phantom* pPhantom)
{
	if (m_pWorld)
	{
	}

	return true;
}
//---------------------------------------------------------------------------
bool WorldHavok::lock()
{
	if (m_pWorld)
	{
		m_pWorld->lock();
		m_pWorld->markForWrite();
	}

	return true;
}
//---------------------------------------------------------------------------
bool WorldHavok::unlock()
{
	if (m_pWorld)
	{
		m_pWorld->unmarkForWrite();
		m_pWorld->unlock();
	}

	return true;
}
//---------------------------------------------------------------------------
bool WorldHavok::castRay(const Ray& ray, Vector4* pPoints, size_t& pointCount)
{
	if (!m_pWorld)	
		return false;

	hkpWorldRayCastInput		input;
	hkpClosestRayHitCollector	collector;

	input.m_from = (hkVector4&)ray.end;
	Vector4 end(ray.end + ray.dir * 10000);
	input.m_to = (hkVector4&)end;
	m_pWorld->castRay(input, collector);

	if (collector.hasHit())
	{
		*pPoints = (ray.end + ray.dir * collector.getHit().m_hitFraction * 10000);
		pointCount = 1;
		return true;
	}

	pointCount = 0;
	return false;
}
//---------------------------------------------------------------------------
bool WorldHavok::castRay(const Ray& ray, void* pC[], size_t& count)
{
	if (!m_pWorld)	
		return false;

	hkpWorldRayCastInput		input;
	hkpAllRayHitCollector	collector;

	input.m_from = (hkVector4&)ray.end;
	Vector4 end(ray.end + ray.dir * 10000);
	input.m_to = (hkVector4&)end;

	m_pWorld->castRay(input, collector);

	HK_DISPLAY_LINE(input.m_from, input.m_to, 0xFFFF00FF);

	const hkArray<hkpWorldRayCastOutput>& hits = collector.getHits();
	for (int i=0; i<hits.getSize() && i<int(count); ++i)
	{
		hkpWorldRayCastOutput hit = hits[i];

		// this returns something that has a getUserData() on it
		hkpWorldObject* pOb = static_cast<hkpWorldObject*>(
			hit.m_rootCollidable->getOwner());

		pC[i] = pOb ? (void*)pOb->getUserData() : 0;
	}

	count = hits.getSize();
	return (count != 0);
}
//---------------------------------------------------------------------------
CollisionFilter* WorldHavok::_getOrCreateCollisionFilter()
{
	if (!m_pFilter)
		m_pFilter = COS_NEW CollisionFilterHavok();

	return m_pFilter;
}
//---------------------------------------------------------------------------
void WorldHavok::_applyCollisionFilter()
{
	CollisionFilterHavok* pCF = static_cast<CollisionFilterHavok*>(m_pFilter);
	if (!pCF)
		return;

	m_pWorld->markForWrite();
	m_pWorld->setCollisionFilter(pCF->getGroupFilter());
	m_pWorld->unmarkForWrite();
}