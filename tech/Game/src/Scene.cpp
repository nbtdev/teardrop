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

#include "Scene.h"
#include "Component_Render.h"
#include "TerrainZone.h"
#include "ZoneObject.h"
#include "LoadListener.h"
#include "SceneUtil.h"
#include "Game/include/Component_Physics.h"
#include "Physics/include/Physics.h"
#include "Physics/include/World.h"
#include "Physics/include/CollisionFilter.h"
#include "Stream/include/FileStream.h"
#include "Resource/include/ResourceManager.h"
#include "Reflection/include/Reflection.h"
#include "Util/include/FourCC.h"
#include "Util/include/StringUtil.h"
#include "Util/include/_String.h"
#include "Util/include/Logger.h"
#include "Util/include/Environment.h"
#include "Math/include/MathUtil.h"
#include "Memory/include/Memory.h"

#include <algorithm>

using namespace CoS;
//---------------------------------------------------------------------------
const FourCC& Scene::RESOURCE_TYPE = FourCC('S','C','N','E');
const static float TIMESTEP = 1.f/60.f;
//---------------------------------------------------------------------------
Scene::Scene()
{
	m_currentZone = Zone::INVALID;
	m_pWorld = 0;
	m_accumTime = 0;
}
//---------------------------------------------------------------------------
Scene::~Scene()
{
}
//---------------------------------------------------------------------------
bool Scene::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
World* Scene::getWorld()
{
	return m_pWorld;
}
//---------------------------------------------------------------------------
void Scene::setupCollisionFilters()
{
	if (!m_pWorld)
		return;

	// set up default collisions
	CollisionFilter* pF = m_pWorld->getOrCreateCollisionFilter();
	pF->disableCollisionBetween(COLLISION_LAYER_RAGDOLL, COLLISION_LAYER_CHARACTER_PROXY);
	pF->disableCollisionBetween(COLLISION_LAYER_RAGDOLL, COLLISION_LAYER_RAGDOLL);
	m_pWorld->applyCollisionFilter();
}
//---------------------------------------------------------------------------
bool Scene::destroy()
{
	for (Zones::iterator it = m_zones.begin(); it != m_zones.end(); ++it)
	{
		(*it).pZone->destroy();
		delete (*it).pZone;
	}

	m_zones.clear();

	if (m_pWorld)
	{
		Physics::removeWorldFromDebugger(m_pWorld);
		m_pWorld->release();
		delete m_pWorld;
		m_pWorld = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
bool Scene::update(float deltaT)
{
	m_accumTime += deltaT;

	while (m_accumTime > TIMESTEP)
	{
		if (m_pWorld)
		{
			if (!m_pWorld->update(TIMESTEP))
				return false;
		}

		m_accumTime -= TIMESTEP;
	}

	// probably should update visible neighbors too?
	m_zones[m_currentZone].pZone->update(deltaT);

	return true;
}
//---------------------------------------------------------------------------
const Vector4& Scene::getAmbientLight() const
{
	return m_zones[m_currentZone].pZone->getAmbient();
}
//---------------------------------------------------------------------------
Zone* Scene::createZone(const char* type, LoadListener* pCB)
{
	if (!type)
		return 0;

	ZoneNode node;

	// use reflection to create the instance
	Reflection::ClassDef* pClassDef = Reflection::ClassDef::findClassDef(type);
	if (!pClassDef)
		return 0;

	// todo: remove the bitwise ID hack
	node.pZone = static_cast<Zone*>(
		pClassDef->createInstance(m_zones.size() | 0x80000000));

#if 0
	if (!_stricmp("terrain", type))
		node.pZone = COS_NEW TerrainZone;
	else
		node.pZone = COS_NEW Zone; // plain zone
#endif // 0

	node.pZone->setId(m_zones.size());
	node.pZone->m_pScene = this;
	node.pZone->m_pLoadListener = pCB;
	m_zones.push_back(node);
	return node.pZone;
}
//---------------------------------------------------------------------------
void Scene::destroyZone(Zone* pZone)
{
	if (!pZone)
	{
		return;
	}

	// remove this guy from the neighbor list first
	size_t id = pZone->getId();
	ZoneNode& zn = m_zones[id];

	for (ZoneNeighbors::iterator it = zn.neighbors.begin();
		it != zn.neighbors.end(); ++it)
	{
		ZoneNode& node = m_zones[*it];
		ZoneNeighbors::iterator n = std::find(node.neighbors.begin(),
			node.neighbors.end(), id);
		if (n != node.neighbors.end())
		{
			node.neighbors.erase(n);
		}
	}

	// then delete the Zone
	delete zn.pZone;

	// then remove the node itself
	std::remove(m_zones.begin(), m_zones.end(), zn);
}
//---------------------------------------------------------------------------
bool Scene::addNeighbor(const Zone *pMe, const Zone* pNeighbor)
{
	if (!pMe || !pNeighbor)
	{
		return false;
	}

	m_zones[pMe->getId()].neighbors.push_back(pNeighbor->getId());
	m_zones[pNeighbor->getId()].neighbors.push_back(pMe->getId());

	return true;
}
//---------------------------------------------------------------------------
Zone* Scene::getCurrentZone() const
{
	if (m_currentZone == Zone::INVALID)
	{
		return 0;
	}

	return m_zones[m_currentZone].pZone;
}
//---------------------------------------------------------------------------
bool Scene::setCurrentZone(const Zone& zone)
{
	m_currentZone = zone.getId();
	return true;
}
//---------------------------------------------------------------------------
bool Scene::getVisibleObjects(
	const Plane* frustum, ZoneObjects& objects) const
{
	// for now, they are *all* visible
	if (m_currentZone <= m_zones.size())
	{
		return m_zones[m_currentZone].pZone->getVisibleObjects(frustum, objects);
	}

	return false;
}
//---------------------------------------------------------------------------
bool Scene::findObjectsWithinRadius(
	ZoneObjects& objects, 
	const Vector4& origin,
	float radius, 
	FilterFunc fn) const
{
	return m_zones[m_currentZone].pZone->getObjectsWithinRadius(
		origin, radius, objects, fn);
}
//---------------------------------------------------------------------------
bool Scene::getIntersectingObjects(
   /*in*/const Ray& ray, 
   /*out*/ZoneObjects& objects,
   bool sort,
   bool precise
   ) const
{
	if (precise)
	{
		// use the collision engine to do the raycast
		if (m_pWorld)
		{
			void* pCollidable[64];
			size_t collidableCount = 64;
			if (m_pWorld->castRay(ray, pCollidable, collidableCount))
			{
				// whatever the collidable is, it should point to a PhysicsComponent
				// instance, and from that we can get the component host and
				// fill out the object list
				objects.reserve(collidableCount);
				for (size_t i=0; i<collidableCount; ++i)
				{
					PhysicsComponent* pComp = 
						static_cast<PhysicsComponent*>(pCollidable[i]);

					if (pComp)
					{
						ZoneObject* pObj = dynamic_cast<ZoneObject*>(pComp->getHost());
						objects.push_back(pObj);
					}
				}

				return (0 != objects.size());
			}
		}
	}
	else
	{
		// use the coarse AABB intersection testing
		return m_zones[m_currentZone].pZone->getIntersectingObjects(
			ray, objects, sort);
	}

	return false;
}
//---------------------------------------------------------------------------
bool Scene::getIntersectionPoints(
								  /*in*/const Ray& ray, 
								  /*out*/Vector4* pPoints,
								  /*inout*/size_t& pointCount
								  ) 
{
	if (m_pWorld)
		return m_pWorld->castRay(ray, pPoints, pointCount);

	return false;
}
//---------------------------------------------------------------------------
void Scene::findAllObjectsOfType(
	/*in*/const Reflection::ClassDef* pClass,
	/*out*/ZoneObjects& list)
{
	list.clear();

	// brute force -- go through all objects and compare their derived 
	// classdef against pClass
	Zone* pZone = m_zones[m_currentZone].pZone;
	for(ZoneObjects::iterator it = pZone->m_objects.begin(); 
		it != pZone->m_objects.end(); ++it)
	{
		if (*it && (*it)->getDerivedClassDef() == pClass)
			list.push_back(*it);
	}
}