/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COLLISIONFILTERHAVOK_INCLUDED)
#define COLLISIONFILTERHAVOK_INCLUDED

#include "Physics/CollisionFilter.h"

class hkpGroupFilter;

namespace Teardrop
{
	class CollisionFilterHavok
		: public CollisionFilter
	{
		hkpGroupFilter* m_pFilter;

	public:
		CollisionFilterHavok();
		~CollisionFilterHavok();

		void enableCollisionBetween(CollisionGroup group1, CollisionGroup group2);
		void enableCollisionBetween(CollisionLayer layer1, CollisionLayer layer2);
		void disableCollisionBetween(CollisionGroup group1, CollisionGroup group2);
		void disableCollisionBetween(CollisionLayer layer1, CollisionLayer layer2);

		hkpGroupFilter* getGroupFilter() const { return m_pFilter; }

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // COLLISIONFILTERHAVOK_INCLUDED
