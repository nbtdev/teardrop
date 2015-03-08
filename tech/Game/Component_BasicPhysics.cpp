/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Component_BasicPhysics.h"
#include "ZoneObject.h"
#include "Zone.h"
#include "Scene.h"
#include "Util/Hash.h"
#include "Util/Environment.h"
#include "Memory/Memory.h"
#include "Memory/Allocators.h"
#include "Physics/Physics.h"
#include "Physics/Body.h"
#include "Physics/Shape.h"
#include "Physics/World.h"
#include <assert.h>

using namespace Teardrop;
using namespace Reflection;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(BasicPhysicsComponent);
//---------------------------------------------------------------------------
BasicPhysicsComponent::BasicPhysicsComponent()
{
	m_pWorld = 0;
    m_pBody = nullptr;
	m_pendingOp = NONE;
}
//---------------------------------------------------------------------------
BasicPhysicsComponent::~BasicPhysicsComponent()
{
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::initialize()
{
	String name(m_assetPath);
	name += "/";
	name += getRigidBodyName();

//	Environment& env = Environment::get();
//	m_hBody = env.pResourceMgr->acquire(
//		Body::RESOURCE_TYPE, name);

	return true;
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::destroy()
{
//	Body* pBody = AutoResPtr<Body>(m_hBody);

//	if (pBody)
//	{
//		pBody->setUserData(0);
//		pBody->release();
//	}

//	Environment::get().pResourceMgr->release(m_hBody);
//	m_hBody = INVALID_RESOURCE_HANDLE;

	return PhysicsComponent::destroy();
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::update(float deltaT)
{
//	Body* pBody = AutoResPtr<Body>(m_hBody);
    Body* pBody = m_pBody;

	if (!pBody)
	{
		return true;
	}

	// hack for now -- need to "tick" the body to give it a chance to init
	Vector4 vel;
	pBody->getLinearVelocity(vel);

	switch(m_pendingOp)
	{
	case ADD:
		if (m_pWorld)
		{
			pBody->setWorldTransform(m_pHost->getTransformWS());
			m_pWorld->add(pBody);
			m_pWorld = 0;
			m_pendingOp = NONE;
			pBody->setUserData(static_cast<PhysicsComponent*>(this));
		}
		break;

	case REMOVE:
		if (m_pWorld)
		{
			m_pWorld->remove(pBody);
			m_pWorld = 0;
			m_pendingOp = NONE;
		}
		break;
	}

	return pBody->update(deltaT);
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::addToWorld(World* pWorld)
{
	m_pWorld = pWorld;
	m_pendingOp = ADD;
	return true;
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::removeFromWorld(World* pWorld)
{
	m_pWorld = pWorld;
	m_pendingOp = REMOVE;
	return true;
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::setBody(const char* /*assetFilename*/)
{
#if 0
    ResourceManager* pMgr = Environment::get().pResourceMgr;
	Body* pBody = AutoResPtr<Body>(m_hBody);

	if (pBody)
	{
		pMgr->release(m_hBody);
	}

	m_hBody = pMgr->acquire(Body::RESOURCE_TYPE, assetFilename);
#endif
	return true;
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::getWorldTransform(Transform& /*xform*/)
{
#if 0
    Body* pBody = AutoResPtr<Body>(m_hBody);

	if (pBody)
	{
		pBody->getWorldTransform(xform);
		return true;
	}
#endif
	return false;
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::setWorldTransform(const Transform& /*xform*/)
{
#if 0
    Body* pBody = AutoResPtr<Body>(m_hBody);

	if (pBody)
	{
		pBody->setWorldTransform(xform);
		return true;
	}
#endif
	return false;
}
//---------------------------------------------------------------------------
void BasicPhysicsComponent::addedToZone(Zone* pZone)
{
	// set up the add of our rigid body to the world
	addToWorld(pZone->getScene()->getWorld());
	// do it now
	update(0);
}
//---------------------------------------------------------------------------
void BasicPhysicsComponent::removeFromZone(Zone* pZone)
{
	// set up the add of our rigid body to the world
	removeFromWorld(pZone->getScene()->getWorld());
	// do it now
	update(0);
}
#if 0
//---------------------------------------------------------------------------
void BasicPhysicsComponent::initializeDebugMesh()
{
    if (m_bDebugMeshInitialized)
		return; // already done

	Body* pBody = AutoResPtr<Body>(m_hBody);
	if (!pBody)
		return;

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
	if (!pBody->getDisplayGeometry(geoms))
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
		//pMtl->setDiffuse(0xffffffff);
		pSubMesh->setMaterial(pMtl, true);
	}

	m_bDebugMeshInitialized = true;
}
#endif
