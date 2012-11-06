/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(WORLDHAVOK_INCLUDED)
#define WORLDHAVOK_INCLUDED

#include "Memory/Allocators.h"
#include "Physics/World.h"
class hkpWorld;
class hkJobThreadPool;
class hkJobQueue;
struct WorldBorderMonitor;

namespace Teardrop
{
	class WorldHavok : public World
	{
		hkpWorld* m_pWorld;
		hkpWorldCinfo& m_ci;
		WorldBorderMonitor* m_pMon;
		hkJobThreadPool* m_pThreadPool;
		hkJobQueue* m_pJobQueue;

	public:
		WorldHavok(hkpWorldCinfo& ci);
		~WorldHavok();

		bool initialize(const AABB& aabb);
		bool release();
		bool update(float deltaT);
		void clear();

		bool lock();
		bool unlock();

		bool add(Body* pBody);
		bool add(Phantom* pPhantom);
		bool remove(Body* pBody);
		bool remove(Phantom* pPhantom);

		hkpWorld* getHavokWorld();
		bool castRay(/*in*/const Ray& ray, /*out*/Vector4* pPoints, /*inout*/size_t& pointCount);
		bool castRay(/*in*/const Ray& ray, /*out*/void* pCollidable[], /*inout*/size_t& count);

		TD_DECLARE_ALLOCATOR();

	protected:
		void _applyCollisionFilter();
		CollisionFilter* _getOrCreateCollisionFilter();
	};
}

#endif // WORLDHAVOK_INCLUDED
