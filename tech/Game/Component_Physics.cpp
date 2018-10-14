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

#include "Component_Physics.h"
#include "ZoneObject.h"
#include "Util/Environment.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(PhysicsComponent);
//---------------------------------------------------------------------------
PhysicsComponent::PhysicsComponent()
{
//	m_bDebugMeshInitialized = false;
}
//---------------------------------------------------------------------------
PhysicsComponent::~PhysicsComponent()
{
}
//---------------------------------------------------------------------------
bool PhysicsComponent::initialize(const char* /*asset*/, const FourCC& /*type*/)
{
	return false;
}
//---------------------------------------------------------------------------
bool PhysicsComponent::destroy()
{
//	releaseDebugMesh();
	return false;
}
//---------------------------------------------------------------------------
bool PhysicsComponent::update(float /*deltaT*/)
{
	return false;
}
//---------------------------------------------------------------------------
bool PhysicsComponent::addToWorld(World* /*pWorld*/)
{
	return false;
}
//---------------------------------------------------------------------------
bool PhysicsComponent::removeFromWorld(World* /*pWorld*/)
{
	return false;
}
//---------------------------------------------------------------------------
bool PhysicsComponent::getWorldTransform(Transform& /*xform*/)
{
	return false;
}
//---------------------------------------------------------------------------
bool PhysicsComponent::setWorldTransform(const Transform& /*xform*/)
{
	return false;
}
//---------------------------------------------------------------------------
void PhysicsComponent::addedToZone(Zone* /*pZone*/)
{
}
//---------------------------------------------------------------------------
void PhysicsComponent::removeFromZone(Zone* /*pZone*/)
{
}
////---------------------------------------------------------------------------
//GfxMeshInstance& PhysicsComponent::getDebugMesh()
//{
//	if (!m_bDebugMeshInitialized)
//		initializeDebugMesh();

//	// update transform on the instance first
//	m_debugMeshInstance.setTransform(m_pHost->getTransformWS());
//	return m_debugMeshInstance;
//}
////---------------------------------------------------------------------------
//void PhysicsComponent::initializeDebugMesh()
//{
//	// implemented by derived classes
//}
////---------------------------------------------------------------------------
//void PhysicsComponent::releaseDebugMesh()
//{
//	// can be overridden by derived classes

//	Environment::get().pResourceMgr->release(
//		m_debugMeshInstance.getMeshHandle());
//}
