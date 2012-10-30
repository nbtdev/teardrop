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

#include "Component_BasicPhysics.h"
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
#include "Physics/include/World.h"
#include <assert.h>

using namespace CoS;
using namespace Reflection;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(BasicPhysicsComponent);
//---------------------------------------------------------------------------
BasicPhysicsComponent::BasicPhysicsComponent()
{
	m_pWorld = 0;
	m_hBody = INVALID_RESOURCE_HANDLE;
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

	Environment& env = Environment::get();
	m_hBody = env.pResourceMgr->acquire(
		Body::RESOURCE_TYPE, name);

	return true;
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::destroy()
{
	Body* pBody = AutoResPtr<Body>(m_hBody);

	if (pBody)
	{
		pBody->setUserData(0);
		pBody->release();
	}

	Environment::get().pResourceMgr->release(m_hBody);
	m_hBody = INVALID_RESOURCE_HANDLE;

	return PhysicsComponent::destroy();
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::update(float deltaT)
{
	Body* pBody = AutoResPtr<Body>(m_hBody);

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
bool BasicPhysicsComponent::setBody(const char* assetFilename)
{
	ResourceManager* pMgr = Environment::get().pResourceMgr;
	Body* pBody = AutoResPtr<Body>(m_hBody);

	if (pBody)
	{
		pMgr->release(m_hBody);
	}

	m_hBody = pMgr->acquire(Body::RESOURCE_TYPE, assetFilename);
	return true;
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::getWorldTransform(Transform& xform)
{
	Body* pBody = AutoResPtr<Body>(m_hBody);

	if (pBody)
	{
		pBody->getWorldTransform(xform);
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool BasicPhysicsComponent::setWorldTransform(const Transform& xform)
{
	Body* pBody = AutoResPtr<Body>(m_hBody);

	if (pBody)
	{
		pBody->setWorldTransform(xform);
		return true;
	}

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

		GfxMaterial* pMtl = COS_NEW GfxMaterial;
		pMtl->setDepthBias(1);
		pMtl->setDiffuse(0xffffffff);
		pSubMesh->setMaterial(pMtl, true);
	}

	m_bDebugMeshInitialized = true;
}
