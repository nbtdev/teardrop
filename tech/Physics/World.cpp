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
