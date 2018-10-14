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
