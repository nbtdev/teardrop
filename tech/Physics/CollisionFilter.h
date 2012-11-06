/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
