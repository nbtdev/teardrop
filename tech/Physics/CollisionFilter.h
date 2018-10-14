/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(COLLISIONFILTER_INCLUDED)
#define COLLISIONFILTER_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	enum CollisionGroup
	{
		COLLISION_GROUP_ALL = 0,
		COLLISION_GROUP_BASE,
		COLLISION_GROUP_LANDSCAPE,
		COLLISION_GROUP_RAGDOLL,
		COLLISION_GROUP_CHARACTER,
		COLLISION_GROUP_PARTICLE,
		COLLISION_GROUP_PROJECTILE,
	};

	enum CollisionLayer
	{
		COLLISION_LAYER_ALL = 0,
		COLLISION_LAYER_CHARACTER_PROXY,
		COLLISION_LAYER_RAGDOLL,
		COLLISION_LAYER_PROP,
		COLLISION_LAYER_TERRAIN,
		COLLISION_LAYER_PROJECTILE,
		COLLISION_LAYER_PARTICLE,
	};

	class CollisionFilter
	{
	public:
		CollisionFilter();
		virtual ~CollisionFilter();

		virtual void enableCollisionBetween(CollisionGroup group1, CollisionGroup group2) = 0;
		virtual void enableCollisionBetween(CollisionLayer layer1, CollisionLayer layer2) = 0;
		virtual void disableCollisionBetween(CollisionGroup group1, CollisionGroup group2) = 0;
		virtual void disableCollisionBetween(CollisionLayer layer1, CollisionLayer layer2) = 0;

		unsigned int getFilter() const { return m_filter; }

		TD_DECLARE_ALLOCATOR();

	protected:
		unsigned int m_filter;
	};
}

#endif // COLLISIONFILTER_INCLUDED
