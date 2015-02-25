/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
