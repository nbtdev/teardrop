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

#include "Component_CharacterPhysics.h"
#include "Physics/include/CharacterProxy.h"
#include "Ragdoll/include/RagdollController.h"
#include "Ragdoll/include/Ragdoll.h"
#include "ZoneObject.h"
#include "Zone.h"
#include "Scene.h"
#include "Util/include/Hash.h"
#include "Util/include/Environment.h"
#include "Resource/include/ResourceManager.h"
#include "Gfx/include/GfxMesh.h"
#include "Gfx/include/GfxSubMesh.h"
#include "Gfx/include/GfxVertexFormat.h"
#include "Gfx/include/GfxVertexData.h"
#include "Gfx/include/GfxMaterial.h"
#include "Memory/include/Memory.h"
#include "Memory/include/Allocators.h"
#include "Physics/include/Physics.h"
#include "Physics/include/Body.h"
#include "Physics/include/Shape.h"
#include <assert.h>

using namespace CoS;
static const float TIMESTEP = 1.f / 60.f;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(CharacterPhysicsComponent);
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
	m_pShape = Physics::createCapsule(
		Vector4(0, 0, 0, 0),
		Vector4(0, 3, 0, 0),
		2
		);

	// most of this should come from data...
	Transform xform(m_pHost->getTransformWS());
	m_pProxy = Physics::createCharacterProxy(
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

		m_pRagdoll = RagdollController::createController();
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
		
	RagdollController::destroyController(m_pRagdoll);

	Physics::destroyCharacterProxy(m_pProxy);
	Physics::destroyShape(m_pShape);

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

		GfxMaterial* pMtl = COS_NEW GfxMaterial;
		pMtl->setDepthBias(1);
		pMtl->setDiffuse(0xffffffff);
		pSubMesh->setMaterial(pMtl, true);
	}

	m_bDebugMeshInitialized = true;
}
