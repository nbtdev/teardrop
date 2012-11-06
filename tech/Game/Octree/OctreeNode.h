/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDOCTREENODE_INCLUDED)
#define TDOCTREENODE_INCLUDED

#include "Math/AABB.h"
#include <list>
#include "Memory/Allocators.h"

namespace Teardrop
{
	class ZoneObject;
	class ZoneObjectList;
	class Plane;
	class Ray;

	struct OctreeNode
	{
		typedef std::list<ZoneObject*> AttachedObjects;

		OctreeNode();
		~OctreeNode();

		AABB dimensions;
		OctreeNode* children[8];
		AttachedObjects objects;

		bool findObjectsWithin(
			/*in*/const Plane* pPlanes, 
			/*in*/size_t numPlanes, 
			/*out*/ZoneObjectList& objects) const;

		bool findIntersectingObjects(
			/*in*/const Ray &ray, 
			/*out*/ZoneObjectList &objects) const;

		void getAllObjects(
			/*out*/ZoneObjectList& objects) const;

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // TDOCTREENODE_INCLUDED
