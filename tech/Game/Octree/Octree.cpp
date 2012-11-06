/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Octree.h"
#include "OctreeNode.h"
#include "ZoneObject.h"
#include "Memory/Memory.h"
#include "Math/MathUtil.h"
#include "Math/Query.h"
#include "Math/Plane.h"
#include <algorithm>

using namespace Teardrop;
static const int MAX_LEVELS = 8;
//---------------------------------------------------------------------------
Octree::Octree()
{
	m_pRoot = 0;
}
//---------------------------------------------------------------------------
Octree::~Octree()
{
}
//---------------------------------------------------------------------------
bool Octree::initialize(const AABB& aabb)
{
	m_pRoot = TD_NEW OctreeNode;
	m_pRoot->dimensions = aabb;

	// leave all children as null for now; we will create them as needed

	return true;
}
//---------------------------------------------------------------------------
static void destroyOctree(OctreeNode* node)
{
	if (node)
	{
		// depth-first destruction
		for (size_t i=0; i<8; ++i)
		{
			if (node->children[i])
			{
				destroyOctree(node->children[i]);
			}
		}
	}

	delete node;
}
//---------------------------------------------------------------------------
bool Octree::destroy()
{
	destroyOctree(m_pRoot);
	return true;
}
//---------------------------------------------------------------------------
void Octree::move(ZoneObject* pObj)
{
	// take the easy route -- remove it from its current node and re-insert 
	// it where it belongs (even if that is the same node)
	ObjectToNodeLUT::iterator it = m_lut.find(pObj);
	if (it != m_lut.end())
	{
		OctreeNode::AttachedObjects::iterator obj = 
			std::find(
				it->second->objects.begin(), 
				it->second->objects.end(), 
				pObj);
		
		if (obj != it->second->objects.end())
		{
			it->second->objects.erase(obj);

			// and then find a new (possibly same) home for it
			insert(pObj);
		}
	}
}
//---------------------------------------------------------------------------
static OctreeNode* insertObject(
	OctreeNode* pNode, 
	ZoneObject* pObject,
	int level)
{
	if (!pNode || !pObject)
	{
		return 0;
	}

	// if we are at our max depth, just put it here and exit
	if (level > MAX_LEVELS)
	{
		pNode->objects.push_back(pObject);
		return pNode;
	}

	// figure out which octant to check
	int index = 0;
	const Vector4& objCenter = pObject->getBoundingBox().center;
	Vector4& nodeCenter = pNode->dimensions.center;

	if (objCenter.x > nodeCenter.x)
	{
		index |= 1;
	}

	if (objCenter.y > nodeCenter.y)
	{
		index |= 2;
	}

	if (objCenter.z > nodeCenter.z)
	{
		index |= 4;
	}

	if (pNode->children[index])
	{
		if (Query::contains(pNode->children[index]->dimensions, pObject->getBoundingBox()))
		{
			return insertObject(pNode->children[index], pObject, ++level);
		}
	}
	else
	{
		// construct an AABB to test against, to see if it would fit in 
		// a new child octant
		AABB aabb;
		aabb.extents = pNode->dimensions.extents / 2.0f;
		aabb.center = pNode->dimensions.center;
		aabb.center.x += (index & 1) ? aabb.extents.x : -aabb.extents.x;
		aabb.center.y += (index & 2) ? aabb.extents.y : -aabb.extents.y;
		aabb.center.z += (index & 4) ? aabb.extents.z : -aabb.extents.z;

		if (Query::contains(aabb, pObject->getBoundingBox()))
		{
			// make a new node at this index and dive down into it with this object
			OctreeNode* pNewNode = TD_NEW OctreeNode;
			pNewNode->dimensions = aabb;
			pNode->children[index] = pNewNode;
			return insertObject(pNewNode, pObject, ++level);
		}
	}

	pNode->objects.push_back(pObject);
	return pNode;
}
//---------------------------------------------------------------------------
bool Octree::insert(ZoneObject* pObject)
{
	OctreeNode* pNode = insertObject(m_pRoot, pObject, 0);
	if (pNode)
	{
		// put it in the lut
		m_lut[pObject] = pNode;
	}

	return true;
}
//---------------------------------------------------------------------------
bool Octree::remove(ZoneObject* pObject)
{
	// find the object's node and remove thyself
	ObjectToNodeLUT::iterator it = m_lut.find(pObject);
	if (it != m_lut.end())
	{
		// find the object in the list
		for (OctreeNode::AttachedObjects::iterator obj = 
				it->second->objects.begin(); obj != it->second->objects.end();
				++obj)
		{
			if (*obj == pObject)
			{
				// remove from both
				it->second->objects.erase(obj);
				m_lut.erase(it);
				return true;
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
static bool testAABBAgainstPlanes(
	const AABB& aabb, 
	const Plane* pPlanes, 
	size_t numPlanes,
	bool& intersects)
{
	intersects = false;

	// check the AABB against all of the planes for either positive halfspace
	// or intersection (both of which are a "yes" test)
	for (size_t i=0; i<numPlanes; ++i)
	{
		bool inside = false;
		bool rtn = Query::AABB_Plane(aabb, pPlanes[i], inside);

		// if the plane and the box are not intersecting, and the box lies
		// fully outside the plane, then we want to return false; otherwise,
		// if it is intersecting or the box lies fully inside the plane, then
		// we want to return true
		intersects |= rtn;

		if (!rtn)
		{
			// if this returned false, then the AABB and plane are not
			// intersecting, so we need to check which halfspace
			if (!inside)
			{
				return false;
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
static void gatherContainedObjects(OctreeNode* pNode, ZoneObjectList& objects)
{
	if (!pNode)
	{
		return;
	}

	// depth-first traversal of all child nodes, to get their attached objects
	for (int i=0; i<8; ++i)
	{
		if (pNode->children[i])
		{
			gatherContainedObjects(pNode->children[i], objects);
		}
	}

	for (OctreeNode::AttachedObjects::iterator it = pNode->objects.begin();
		it != pNode->objects.end(); ++it)
	{
		objects.push_back(*it);
	}
}
//---------------------------------------------------------------------------
static void testChildren(
	OctreeNode* pNode,
	const Plane* pPlanes, 
	size_t numPlanes, 
	ZoneObjectList& objects)
{
	// check each existing child of the root node, for starters
	for (int i=0; i<8; ++i)
	{
		if (pNode->children[i])
		{
			bool intersects;
			bool rtn = testAABBAgainstPlanes(
				pNode->children[i]->dimensions, 
				pPlanes, 
				numPlanes, 
				intersects);

			if (rtn)
			{
				if (intersects)
				{
					// then we need to refine the list under this node
					testChildren(pNode->children[i], pPlanes, numPlanes, objects);

					// and also check each object attached to this node...
					for (OctreeNode::AttachedObjects::iterator it = pNode->objects.begin();
						it != pNode->objects.end(); ++it)
					{
						if (testAABBAgainstPlanes((*it)->getBoundingBox(),
							pPlanes, numPlanes, intersects))
						{
							// at this point, draw intersecting objects
							objects.push_back(*it);
						}
					}
				}
				else
				{
					// otherwise, everything under this node is in view
					gatherContainedObjects(pNode->children[i], objects);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
bool Octree::findObjectsWithin(
	/*in*/const Plane* pPlanes, 
	/*in*/size_t numPlanes, 
	/*out*/ZoneObjectList& objects) const
{
	if (!m_pRoot)
	{
		return false;
	}

	return m_pRoot->findObjectsWithin(pPlanes, numPlanes, objects);
}
//---------------------------------------------------------------------------
bool Octree::findIntersectingObjects(
	const Ray &ray, 
	ZoneObjectList &objects) const
{
	if (!m_pRoot)
	{
		return false;
	}

	return m_pRoot->findIntersectingObjects(ray, objects);
}
//---------------------------------------------------------------------------
bool Octree::findObjectsWithinRadius(
	/*in*/const Vector4& /*origin*/,
	/*in*/float /*radius*/,
	/*out*/ZoneObjectList& objects) const
{
	// for now, just get everything in the scene
	m_pRoot->getAllObjects(objects);
	return true;
}
