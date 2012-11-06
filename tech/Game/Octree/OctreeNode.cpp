/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "OctreeNode.h"
#include "ZoneObject.h"
#include "Math/MathUtil.h"
#include "Math/Query.h"
#include "Math/Plane.h"
#include "SpatialPartitioning.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
OctreeNode::OctreeNode()
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;
	children[4] = 0;
	children[5] = 0;
	children[6] = 0;
	children[7] = 0;
}
//---------------------------------------------------------------------------
OctreeNode::~OctreeNode()
{
}
//---------------------------------------------------------------------------
bool OctreeNode::findObjectsWithin(
	/*in*/const Plane* pPlanes, 
	/*in*/size_t numPlanes, 
	/*out*/ZoneObjectList& objectsOut) const
{
	bool intersects = false;

	for (size_t i=0; i<numPlanes; ++i)
	{
		bool insideTest;
		bool rtn = Query::AABB_Plane(
			dimensions,
			pPlanes[i],
			insideTest);

		intersects |= rtn;

		// if we lie fully outside a plane, bail early
		if (!rtn)
		{
			if (!insideTest)
			{
				return false;
			}
		}
	}

	// at this point, either we lie fully inside the planes, or
	// we intersect one or more of them. If we lie fully inside, 
	// then we need simply to get all of the objects attached to 
	// this node as well as all child nodes; if we intersect, then
	// we need to repeat this testing for each child node
	if (intersects)
	{
		// test each child node
		for (int i=0; i<8; ++i)
		{
			if (children[i])
			{
				children[i]->findObjectsWithin(
					pPlanes, numPlanes, objectsOut);
			}
		}

		// and also each object attached to this node
		for (AttachedObjects::const_iterator it = objects.begin();
			it != objects.end(); ++it)
		{
			ZoneObject* pObj = *it;
			if (pObj)
			{
				bool valid = true;

				for (size_t i=0; i<numPlanes; ++i)
				{
					bool insideTest = false;
					bool rtn = Query::AABB_Plane(
						pObj->getBoundingBox(),
						pPlanes[i],
						insideTest);

					// the only thing that will prevent this object from being gathered is 
					// if it lies fully outside for any of these tests
					if (!rtn && !insideTest)
					{
						valid = false;
						break;
					}
				}
				
				// at this point, render intersecting objects too
				if (valid)
				{
					objectsOut.push_back(pObj);
				}
			}
		}
	}
	else
	{
		// get all attached objects
		getAllObjects(objectsOut);
	}

	return true;
}
//---------------------------------------------------------------------------
void OctreeNode::getAllObjects(
	/*out*/ZoneObjectList& objectList) const
{
	// first get our own objects, if any
	for (AttachedObjects::const_iterator it = objects.begin();
		it != objects.end(); ++it)
	{
		objectList.push_back(*it);
	}

	// and then get the objects at each child node, recursively
	for (int i=0; i<8; ++i)
	{
		if (children[i])
		{
			children[i]->getAllObjects(objectList);
		}
	}
}
//---------------------------------------------------------------------------
bool OctreeNode::findIntersectingObjects(
	/*in*/const Ray &ray, 
	/*out*/ZoneObjectList &objs) const
{
	bool found = false;

	// test each child node
	for (int i=0; i<8; ++i)
	{
		if (children[i])
		{
			found |= children[i]->findIntersectingObjects(ray, objs);
		}
	}

	// and also each object attached to this node
	for (AttachedObjects::const_iterator it = objects.begin();
		it != objects.end(); ++it)
	{
		ZoneObject* pObj = *it;
		if (pObj)
		{
			if (Query::Ray_AABB(ray,
				pObj->getBoundingBox()))
			{
				found = true;
				objs.push_back(pObj);
			}
		}
	}

	return found;
}
