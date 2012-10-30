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

#if !defined(WORLD_INCLUDED)
#define WORLD_INCLUDED

#include "Memory/include/Allocators.h"
#include "Math/include/AABB.h"
#include "Math/include/Vector4.h"

namespace CoS
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

		COS_DECLARE_ALLOCATOR();

	protected:
		AABB m_aabb;
		Vector4 m_gravity;
		CollisionFilter* m_pFilter;

		virtual void _applyCollisionFilter() = 0;
		virtual CollisionFilter* _getOrCreateCollisionFilter() = 0;
	};
}

#endif // WORLD_INCLUDED
