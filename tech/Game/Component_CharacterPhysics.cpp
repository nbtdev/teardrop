/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Component_CharacterPhysics.h"
#include "Physics/CharacterProxy.h"
#include "Ragdoll/RagdollController.h"
#include "Ragdoll/RagdollSystem.h"
#include "Ragdoll/Ragdoll.h"
#include "ZoneObject.h"
#include "Zone.h"
#include "Scene.h"
#include "Util/Hash.h"
#include "Util/Environment.h"
#include "Util/SystemManager.h"
#include "Resource/ResourceManager.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxVertexFormat.h"
#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxMaterial.h"
#include "Memory/Memory.h"
#include "Memory/Allocators.h"
#include "Physics/Physics.h"
#include "Physics/Body.h"
#include "Physics/Shape.h"
#include <assert.h>

using namespace Teardrop;
static const float TIMESTEP = 1.f / 60.f;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(CharacterPhysicsComponent);
//---------------------------------------------------------------------------
CharacterPhysicsComponent::CharacterPhysicsComponent()
{
	m_pProxy = 0;
	m_pRagdoll = 0;
	m_pShape = 0;
	m_gravVel = 0;
	m_accumTime = 0;
	m_bRagdollNeedsAdded = false;
	m_pWorld = 0;
}
//---------------------------------------------------------------------------
CharacterPhysicsComponent::~CharacterPhysicsComponent()
{
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::initialize(
	const char* /*asset*/, const FourCC& /*type*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::initialize()
{
	PhysicsSystem* pSys = static_cast<PhysicsSystem*>(
		Environment::get().pSystemMgr->getActiveSystem(System::SYSTEM_PHYSICS));
	m_pShape = pSys->createCapsule(
		Vector4(0, 0, 0, 0),
		Vector4(0, 3, 0, 0),
		2
		);

	// most of this should come from data...
	Transform xform(m_pHost->getTransformWS());
	m_pProxy = pSys->createCharacterProxy(
		m_pShape,
		xform.trans,
		1,
		1,
		1);

	m_pShape->release();

	// create a ragdoll for this character if one is specified
	if (getRagdollName().length())
	{
		String pathname(m_assetPath);
		pathname += "/";
		pathname += getRagdollName();

		HResource hRagdoll = Environment::get().pResourceMgr->acquire(
			Ragdoll::RESOURCE_TYPE, pathname);

		RagdollSystem* pSys = static_cast<RagdollSystem*>(
			Environment::get().pSystemMgr->getActiveSystem(System::SYSTEM_RAGDOLL));
		m_pRagdoll = pSys->createController();
		m_pRagdoll->initialize(hRagdoll);
		m_pRagdoll->setUserData(static_cast<PhysicsComponent*>(this));
	}

	return true;
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::destroy()
{
	if (m_pRagdoll)
		m_pRagdoll->destroy();
		
	RagdollSystem* pRagdollSys = static_cast<RagdollSystem*>(
		Environment::get().pSystemMgr->getActiveSystem(System::SYSTEM_RAGDOLL));
	pRagdollSys->destroyController(m_pRagdoll);

	PhysicsSystem* pSys = static_cast<PhysicsSystem*>(
		Environment::get().pSystemMgr->getActiveSystem(System::SYSTEM_PHYSICS));
	pSys->destroyCharacterProxy(m_pProxy);
	pSys->destroyShape(m_pShape);

	return true;
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::update(float deltaT)
{
	if (m_bRagdollNeedsAdded)
	{
		m_bRagdollNeedsAdded = !(m_pRagdoll->addToWorld(m_pWorld));
		if (!m_bRagdollNeedsAdded)
			m_pWorld = 0;
	}

	m_accumTime += deltaT;
	bool rtn = false;
	while (m_accumTime > TIMESTEP)
	{
		// Testing
		if (!m_pProxy->isSupported())
		{
			Vector4 vel;
			m_pProxy->getLinearVelocity(vel);
			m_gravVel += -9.8f * TIMESTEP;
			vel.y = m_gravVel;
			m_pProxy->setLinearVelocity(vel);
		}
		else
		{
			m_gravVel = 0;
		}
		// End Testing

		m_accumTime -= TIMESTEP;
		rtn = m_pProxy->update(TIMESTEP);
	}

	return rtn;
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::addToWorld(World* pWorld)
{
	m_pProxy->addToWorld(pWorld);

	// the ragdoll is coming from a resource, so may not be ready by the
	// time our proxy is added -- mark it if this is the case so we can 
	// retry later
	if (m_pRagdoll)
	{
		m_bRagdollNeedsAdded = !(m_pRagdoll->addToWorld(pWorld));
		if (m_bRagdollNeedsAdded) 
			m_pWorld = pWorld;
	}
	return true;
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::removeFromWorld(World* pWorld)
{
	if (m_pRagdoll)
		m_pRagdoll->removeFromWorld(pWorld);
	m_pProxy->removeFromWorld(pWorld);
	return true;
}
//---------------------------------------------------------------------------
void CharacterPhysicsComponent::setLinearVelocity(const Vector4& vel)
{
	Vector4 v(vel);
	v.y = m_gravVel;
	m_pProxy->setLinearVelocity(v);
}
//---------------------------------------------------------------------------
void CharacterPhysicsComponent::setAngularVelocity(const Vector4& vel)
{
	m_pProxy->setAngularVelocity(vel);
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::getWorldTransform(Transform& xform)
{
	m_pProxy->getWorldTransform(xform);
	return true;
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::setWorldTransform(const Transform& xform)
{
	m_pProxy->setWorldTransform(xform);
	return true;
}
//---------------------------------------------------------------------------
void CharacterPhysicsComponent::addedToZone(Zone* pZone)
{
	addToWorld(pZone->getScene()->getWorld());
}
//---------------------------------------------------------------------------
void CharacterPhysicsComponent::removeFromZone(Zone* pZone)
{
	removeFromWorld(pZone->getScene()->getWorld());
}
//---------------------------------------------------------------------------
bool CharacterPhysicsComponent::isSupported() const
{
	return m_pProxy->isSupported();
}
//---------------------------------------------------------------------------
void CharacterPhysicsComponent::driveRagdoll(
	float deltaT, AnimationBlender* pBlender, const Transform& xform)
{
	if (m_pRagdoll) 
		m_pRagdoll->drive(deltaT, pBlender, xform);
}
//---------------------------------------------------------------------------
void CharacterPhysicsComponent::initializeDebugMesh()
{
	if (m_bDebugMeshInitialized)
		return; // already done

	if (m_debugMeshInstance.getMeshHandle() == INVALID_RESOURCE_HANDLE)
	{
		// request one
		m_debugMeshInstance.setMeshHandle(
			Environment::get().pResourceMgr->acquire(GfxMesh::RESOURCE_TYPE));
		m_debugMeshInstance.setTransform(Transform::IDENTITY);
	}

	GfxMesh* pMesh = AutoResPtr<GfxMesh>(m_debugMeshInstance.getMeshHandle());
	if (!pMesh)
		return;

	Body::DisplayGeometries geoms;
	if (!m_pRagdoll->getDisplayGeometry(geoms))
		return;

	// at this point we should have valid display geometry, and a valid mesh, so 
	// we can set up the mesh instance with that data
	for (Body::DisplayGeometries::iterator it = geoms.begin(); 
		it != geoms.end(); ++it)
	{
		Body::DisplayGeometry& dg = *it;

		// each one is a submesh
		GfxSubMesh* pSubMesh = pMesh->createSubMesh();
		pSubMesh->setPrimitiveType(LINELIST);

		GfxVertexFormat fmt;
		GfxVertexFormat::Element elem;
		elem.semantic = POSITION;
		elem.type = FLOAT4;
		fmt.addElement(elem);
		pSubMesh->setVertexFormat(Environment::get(), fmt);

		size_t strm;
		GfxVertexData* pVD = pSubMesh->createVertexData(
			strm,
			Environment::get(),
			fmt.getVertexSize(),
			dg.size(),
			(GfxVertexData::CreationFlags)(GfxVertexData::STATIC | GfxVertexData::WRITE_ONLY));

		void* pVB = pVD->lock(false);
		memcpy(pVB, &dg[0], fmt.getVertexSize() * dg.size());
		pVD->unlock();

		GfxMaterial* pMtl = TD_NEW GfxMaterial;
		pMtl->setDepthBias(1);
		pMtl->setDiffuse(0xffffffff);
		pSubMesh->setMaterial(pMtl, true);
	}

	m_bDebugMeshInitialized = true;
}
