/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(WORLD_INCLUDED)
#define WORLD_INCLUDED

#include "Memory/Allocators.h"
#include "Math/AABB.h"
#include "Math/Vector4.h"

namespace Teardrop
{
	class Body;
	class Phantom;
	class Ray;
	class Vector4;
	class CollisionFilter;

	class World
	{
	public:
		World();
		virtual ~World();

		virtual bool initialize(const AABB& aabb);
		virtual bool release();
		virtual bool update(float deltaT);
		virtual void clear();

		virtual bool lock();
		virtual bool unlock();

		virtual bool add(Body* pBody);
		virtual bool add(Phantom* pPhantom);
		virtual bool remove(Body* pBody);
		virtual bool remove(Phantom* pPhantom);

		void setGravity(const Vector4& gravity);
		void getGravity(Vector4& gravity) const;

		virtual void applyCollisionFilter();
		virtual CollisionFilter* getOrCreateCollisionFilter();

		virtual bool castRay(/*in*/const Ray& ray, /*out*/Vector4* pPoints, /*inout*/size_t& pointCount);
		virtual bool castRay(/*in*/const Ray& ray, /*out*/void* pCollidables[], /*inout*/size_t& count);

		TD_DECLARE_ALLOCATOR();

	protected:
		AABB m_aabb;
		Vector4 m_gravity;
		CollisionFilter* m_pFilter;

		virtual void _applyCollisionFilter() = 0;
		virtual CollisionFilter* _getOrCreateCollisionFilter() = 0;
	};
}

#endif // WORLD_INCLUDED
