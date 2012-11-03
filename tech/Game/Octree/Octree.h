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

#if !defined(COSOCTREE_INCLUDED)
#define COSOCTREE_INCLUDED

#include "SpatialPartitioning.h"
#include "Memory/Allocators.h"
#include <map>

namespace CoS
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

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // COSOCTREE_INCLUDED
