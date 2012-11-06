/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDOCTREE_INCLUDED)
#define TDOCTREE_INCLUDED

#include "SpatialPartitioning.h"
#include "Memory/Allocators.h"
#include <map>

namespace Teardrop
{
	class AABB;
	struct OctreeNode;

	class Octree : public SpatialPartitioning
	{
		OctreeNode* m_pRoot;
		typedef std::map<ZoneObject*, OctreeNode*> ObjectToNodeLUT;
		ObjectToNodeLUT m_lut;

	public:
		Octree();
		~Octree();

		//! SpatialPartitioning implementation
		bool initialize(const AABB& aabb);
		bool destroy();
		bool insert(ZoneObject* pObject);
		bool remove(ZoneObject* pObject);
		void move(ZoneObject* pObject);
		bool findObjectsWithin(
			/*in*/const Plane* pPlanes, 
			/*in*/size_t numPlanes, 
			/*out*/ZoneObjectList& objects) const;
		bool findIntersectingObjects(
			/*in*/const Ray& ray, 
			/*out*/ZoneObjectList& objects) const;
		bool findObjectsWithinRadius(
			/*in*/const Vector4& origin,
			/*in*/float radius,
			/*out*/ZoneObjectList& objects) const;

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // TDOCTREE_INCLUDED
