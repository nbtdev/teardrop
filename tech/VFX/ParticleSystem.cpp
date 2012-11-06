/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ParticleSystem.h"
#include "ParticleSystemRenderer.h"
#include "Affector.h"
#include "Emitters/RandomPointEmitter.h"
#include "Game/Component_Render.h"
#include "Util/Environment.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(ParticleSystem);
//---------------------------------------------------------------------------
ParticleSystem::ParticleSystem()
{
	m_pScript = 0;
}
//---------------------------------------------------------------------------
ParticleSystem::~ParticleSystem()
{

}
//---------------------------------------------------------------------------
bool ParticleSystem::initialize()
{
	if (!ZoneObject::initialize())
		return false;

	// early-out if we have no components to initialize (usually true on a server)
	if (!m_components.size())
		return true;

	m_particles.resize(getSystemQuota());

	std::vector<Component*> comp;
	comp.resize(m_components.size());

	// initialize our Emitters 
	size_t numEmitters = 
		findComponents(Emitter::getClassDef(), &comp[0], m_components.size(), true);

	for (size_t i=0; i<numEmitters; ++i)
	{
		Emitter* pEmitter = static_cast<Emitter*>(comp[i]);
		pEmitter->setParticleAllocator(&m_particles);
	}

	// setup our RenderComponent, if any
	RenderComponent* pRenderable = 0;
	if (findComponents(
		RenderComponent::getClassDef(), (Component**)&pRenderable))
	{
		pRenderable->setShadowCaster(false);
		pRenderable->setShadowReceiver(false);
	}

	return true;
}
//---------------------------------------------------------------------------
bool ParticleSystem::destroy()
{
	return ZoneObject::destroy();
}
//---------------------------------------------------------------------------
void ParticleSystem::setCameraParameters(
	const Vector4& camPos, const Quaternion& camRot)
{
	ParticleSystemRenderer* pRenderer = 0;
	if (findComponents(
		ParticleSystemRenderer::getClassDef(), (Component**)&pRenderer))
	{
		pRenderer->setCameraParameters(camPos, camRot);
	}
}
//---------------------------------------------------------------------------
bool ParticleSystem::update(float deltaT)
{
	// early-out if we have no components to update (usually true on a server)
	if (!m_components.size())
		return true;

	std::vector<Component*> comp;
	comp.resize(m_components.size());

	size_t numEmitters = 
		findComponents(Emitter::getClassDef(), &comp[0], m_components.size(), true);

	ParticleSystemRenderer* pRenderer = 0;
	if (!findComponents(
		ParticleSystemRenderer::getClassDef(), (Component**)&pRenderer))
	{
		return false;
	}

	pRenderer->getParticleList().clear();
	for (size_t i=0; i<numEmitters; ++i)
	{
		Emitter* pEmitter = static_cast<Emitter*>(comp[i]);
		pEmitter->update(deltaT);
		pEmitter->appendParticlesToRender(pRenderer->getParticleList());
	}

	size_t numAffectors = 
		findComponents(Affector::getClassDef(), &comp[0], m_components.size(), true);

	for (size_t i=0; i<numAffectors; ++i)
	{
		Affector* pAffector = static_cast<Affector*>(comp[i]);
		pAffector->update(deltaT);
	}
	
	// update the particle system renderer
	pRenderer->update(deltaT);

	// set our mesh instance on our RenderComponent, if any
	RenderComponent* pRenderable = 0;
	if (findComponents(
		RenderComponent::getClassDef(), (Component**)&pRenderable))
	{
		pRenderable->setMeshInstance(pRenderer->getMeshInstance());
	}

	return ZoneObject::update(deltaT);
}
