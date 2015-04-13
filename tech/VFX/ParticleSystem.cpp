/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
//		pRenderable->setMeshInstance(pRenderer->getMeshInstance());
	}

	return ZoneObject::update(deltaT);
}
