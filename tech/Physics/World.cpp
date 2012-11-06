/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "World.h"
#include "Util/Environment.h"
#include "Util/Logger.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
World::World()
{
	m_gravity = Vector4(0, -9.8f, 0, 0);
	m_pFilter = 0;
}
//---------------------------------------------------------------------------
World::~World()
{
}
//---------------------------------------------------------------------------
bool World::initialize(const AABB& aabb)
{
	m_aabb = aabb;
	return true;
}
//---------------------------------------------------------------------------
bool World::release()
{
	return true;
}
//---------------------------------------------------------------------------
bool World::update(float /*deltaT*/)
{
	return true;
}
//---------------------------------------------------------------------------
void World::clear()
{
}
//---------------------------------------------------------------------------
bool World::add(Body* /*pBody*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool World::add(Phantom* /*pPhantom*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool World::remove(Body* /*pBody*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool World::remove(Phantom* /*pPhantom*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool World::lock()
{
	return true;
}
//---------------------------------------------------------------------------
bool World::unlock()
{
	return true;
}
//---------------------------------------------------------------------------
void World::setGravity(const Vector4& gravity)
{
	m_gravity = gravity;
}
//---------------------------------------------------------------------------
void World::getGravity(Vector4& gravity) const
{
	gravity = m_gravity;
}
//---------------------------------------------------------------------------
bool World::castRay(const Ray& /*ray*/, Vector4* /*pPoints*/, size_t& /*pointCount*/)
{
	return false;
}
//---------------------------------------------------------------------------
bool World::castRay(const Ray& /*ray*/, void** /*pC*/, size_t& /*count*/)
{
	return false;
}
//---------------------------------------------------------------------------
void World::applyCollisionFilter()
{
	if (!m_pFilter)
		return;

	_applyCollisionFilter();
}
//---------------------------------------------------------------------------
CollisionFilter* World::getOrCreateCollisionFilter()
{
	return _getOrCreateCollisionFilter();
}
