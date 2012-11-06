/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDSPATIALPARTITIONING_INCLUDED)
#define TDSPATIALPARTITIONING_INCLUDED

#include <list>

namespace Teardrop
{
	class AABB;
	class Plane;
	class Ray;
	class ZoneObject;
	class Vector4;

	class ZoneObjectList : public std::list<ZoneObject*> {};

	class SpatialPartitioning
	{
	public:
		virtual ~SpatialPartitioning(){}

		//! construct the partitioning scheme with the provided world dimensions
		virtual bool initialize(const AABB& aabb) = 0;
		//! destroy the spatial partitioning, freeing any used resources
		virtual bool destroy() = 0;
		//! insert an object into the space
		virtual bool insert(ZoneObject* pObject) = 0;
		//! remove an object from the space
		virtual bool remove(ZoneObject* pObject) = 0;
		//! an object moved in the space; repartition if needed
		virtual void move(ZoneObject* pObject) = 0;
		//! find all visible objects within the space bounded by the provided planes, return in the object list
		virtual bool findObjectsWithin(
			/*in*/const Plane* pPlanes, 
			/*in*/size_t numPlanes, 
			/*out*/ZoneObjectList& objects) const = 0;
		virtual bool findIntersectingObjects(
			/*in*/const Ray& ray, 
			/*out*/ZoneObjectList& objects) const = 0;
		virtual bool findObjectsWithinRadius(
			/*in*/const Vector4& origin,
			/*in*/float radius,
			/*out*/ZoneObjectList& objects) const = 0;
	};
}

#endif // TDSPATIALPARTITIONING_INCLUDED
