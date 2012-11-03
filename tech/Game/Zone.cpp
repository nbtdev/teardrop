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

#include "Zone.h"
#include "Component_Physics.h"
#include "LoadListener.h"
#include "Stream/Stream.h"
#include "Memory/Memory.h"
#include "ZoneObject.h"
#include "SpawnPoint.h"
#include "Octree/Octree.h"
#include "Gfx/GfxCamera.h"
#include "Physics/World.h"
#include "Math/Ray.h"
#include <algorithm>

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(Zone);
static const char* ZONE_TYPE = "generic";
const SpawnPoint& Zone::INVALID_SPAWN = SpawnPoint();
//---------------------------------------------------------------------------
Zone::Zone()
{
	m_id = INVALID;
	m_pSpace = 0;
	m_pWorld = 0;
	m_pScene = 0;
	m_pLoadListener = 0;
	m_name[0] = 0;
}
//---------------------------------------------------------------------------
Zone::~Zone()
{
}
//---------------------------------------------------------------------------
const char* Zone::getType() const
{
	return ZONE_TYPE;
}
//---------------------------------------------------------------------------
const ZoneObjects& Zone::getZoneObjects() const
{
	return m_objects;
}
//---------------------------------------------------------------------------
bool Zone::initialize(World* pWorld)
{
	// figure out the AABB from the dimensional properties
	m_aabb.center = getOrigin();
	m_aabb.extents = getDimensions() / 2;
	// initialize octree
	m_pSpace = COS_NEW Octree;
	m_pSpace->initialize(m_aabb);
	m_pWorld = pWorld;
	return true;
}
//---------------------------------------------------------------------------
bool Zone::destroy()
{
	if (m_pSpace)
	{
		m_pSpace->destroy();
		delete m_pSpace;
		m_pSpace = 0;
	}

	for (size_t i=0; i < m_objects.size(); ++i)
	{
		if (m_objects[i])
		{
			const_cast<ZoneObject*>(m_objects[i])->removeFromZone(this);
			const_cast<ZoneObject*>(m_objects[i])->destroy();
			delete (m_objects[i]);
		}
	}
	return true;
}
//---------------------------------------------------------------------------
void Zone::update(float deltaT)
{
	// go through the dynamic objects and update() them
	for (DynamicObjects::iterator it = m_dynamicObjects.begin();
		it != m_dynamicObjects.end(); ++it)
	{
		ZoneObject* pObj = m_objects[*it];
		if (pObj)
			pObj->update(deltaT);
	}
}
//---------------------------------------------------------------------------
const AABB& Zone::getAABB() const
{
	return m_aabb;
}
//---------------------------------------------------------------------------
size_t Zone::getId() const
{
	return m_id;
}
//---------------------------------------------------------------------------
void Zone::setId(size_t id)
{
	m_id = id;
}
//---------------------------------------------------------------------------
void Zone::addSpawnPoint(SpawnPoint* /*pSpawn*/)
{
}
//---------------------------------------------------------------------------
size_t Zone::getNumSpawnPoints() const
{
	return 0;
}
//---------------------------------------------------------------------------
const SpawnPoint& Zone::getSpawnPoint(size_t /*index*/)
{
	return INVALID_SPAWN;
}
//---------------------------------------------------------------------------
void Zone::freeSpawnPoint(size_t /*index*/)
{
}
//---------------------------------------------------------------------------
const SpawnPoint** Zone::getSpawnPoints() const
{
	return 0;
}
//---------------------------------------------------------------------------
Scene* Zone::getScene() const
{
	return m_pScene;
}
//---------------------------------------------------------------------------
void Zone::setWorldPosition(const Vector4& pos)
{
	m_aabb.center = pos;
	setOrigin(pos);
}
//---------------------------------------------------------------------------
void Zone::setZoneDimensions(const Vector4& dimensions)
{
	m_aabb.extents = dimensions / 2.0f;
	setDimensions(dimensions);
}
//---------------------------------------------------------------------------
void Zone::setDataRootPath(const String& path)
{
	m_dataRoot = path;
} 
//---------------------------------------------------------------------------
void Zone::addShadowCastingLight(DynamicLight* pLight)
{
	for (ShadowCastingLights::iterator it = m_shadowCastingLights.begin();
		it != m_shadowCastingLights.end(); ++it)
	{
		if (pLight == *it)
			return;
	}

	m_shadowCastingLights.push_back(pLight);
} 
//---------------------------------------------------------------------------
void Zone::removeShadowCastingLight(DynamicLight* pLight)
{
	for (ShadowCastingLights::iterator it = m_shadowCastingLights.begin();
		it != m_shadowCastingLights.end(); ++it)
	{
		if (pLight == *it)
		{
			m_shadowCastingLights.erase(it);
			return;
		}
	}
} 
//---------------------------------------------------------------------------
size_t Zone::getNumShadowCastingLights()
{
	return m_shadowCastingLights.size();
} 
//---------------------------------------------------------------------------
DynamicLight* Zone::getShadowCastingLight(size_t idx)
{
	if (idx >= m_shadowCastingLights.size())
		return 0;

	return m_shadowCastingLights[idx];
} 
//---------------------------------------------------------------------------
String Zone::toString()
{
	return getName();
} 
//---------------------------------------------------------------------------
void Zone::objectMoved(ZoneObject* pObject)
{
	if (m_pSpace)
		m_pSpace->move(pObject);
} 
//---------------------------------------------------------------------------
bool Zone::removeObject(ZoneObject* pObject)
{
	if (m_pSpace)
	{
		pObject->removeFromZone(this);
		m_objects[pObject->getObjectId()] = 0;

		// if object was dynamic, remove it from the dynamic update list
		DynamicObjects::iterator it = 
			std::find(
				m_dynamicObjects.begin(), 
				m_dynamicObjects.end(), 
				pObject->getObjectId());

		if (it != m_dynamicObjects.end())
			m_dynamicObjects.erase(it);

		return m_pSpace->remove(pObject);
	}
	
	return false;
}
//---------------------------------------------------------------------------
bool Zone::addObject(ZoneObject* pObject)
{
	if (m_pSpace)
	{
		if (m_pSpace->insert(pObject))
		{
			// maybe someday we'll want to be a bit smarter about this...
			pObject->m_objectId = m_objects.size();
			m_objects.push_back(pObject);

			// if this is a dynamic object, add it to that list too
			if (pObject->isDynamic())
			{
				m_dynamicObjects.push_back(pObject->m_objectId);
			}

			m_aabb.merge(pObject->getBoundingBox());
			pObject->addedToZone(this);
			return true;
		}
	}
	
	return false;
}
//---------------------------------------------------------------------------
bool Zone::getVisibleObjects(const Plane* frustum, ZoneObjects& objects) const
{
	// clear the incoming list
	objects.clear();

	// get the list of visible objects from the space partitioner
	ZoneObjectList objectList;
	if (m_pSpace)
	{
		if (m_pSpace->findObjectsWithin(
			frustum,
			6,
			objectList
			))
		{
			// copy the object to the provided list
			objects.resize(objectList.size());
			size_t i = 0;
			for(ZoneObjectList::iterator it = objectList.begin();
				it != objectList.end(); ++it)
			{
				objects[i++] = *it;
			}

			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
static bool UDless(const ZoneObject* p1, const ZoneObject* p2) 
{
	return (p1->getDistSq() < p2->getDistSq());
}
//---------------------------------------------------------------------------
bool Zone::getIntersectingObjects(
	/*in*/const Ray& ray, 
	/*out*/ZoneObjects& objects,
	bool sort
	) const
{
	objects.clear();

	if (m_pSpace)
	{
		ZoneObjectList objlist;
		bool rtn = m_pSpace->findIntersectingObjects(ray, objlist);

		objects.resize(objlist.size());

		int i=0;
		for (ZoneObjectList::iterator it = objlist.begin(); 
			it != objlist.end(); ++it)
		{
			ZoneObject* pObj = *it;
			pObj->setDistanceSqFrom(ray.end);
			objects[i++] = *it;
		}

		if (sort)
		{
			std::sort(objects.begin(), objects.end(), UDless);
		}

		return rtn;
	}
	
	return false;
}
//---------------------------------------------------------------------------
bool Zone::getObjectsWithinRadius(
	const Vector4& origin, 
	float radius, 
	/*out*/ZoneObjects& objects,
	FilterFunc fn) const
{
	objects.clear();

	if (m_pSpace)
	{
		ZoneObjectList objlist;
		bool rtn = m_pSpace->findObjectsWithinRadius(
			origin, radius, objlist);

		objects.reserve(objlist.size());

		for (ZoneObjectList::iterator it = objlist.begin(); 
			it != objlist.end(); ++it)
		{
			// fn == 0 indicates "we want them all"; fn != 0
			// indicates "call me to check"
			ZoneObject* pObj = *it;
			if ((!fn) || (fn && fn(pObj)))
				objects.push_back(pObj);
		}

		return rtn;
	}
	
	return false;
}
