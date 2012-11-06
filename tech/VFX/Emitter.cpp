/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Emitter.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(Emitter);
//---------------------------------------------------------------------------
Emitter::Emitter()
{
	m_particleAllocator = 0;

	// emitter starts immediately, by default
	start(); 
}
//---------------------------------------------------------------------------
Emitter::~Emitter()
{

}
//---------------------------------------------------------------------------
void Emitter::onInstanceCreated()
{
	setServerComponent(false); // not needed on a server
}
//---------------------------------------------------------------------------
bool Emitter::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool Emitter::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool Emitter::update(float deltaT)
{
	m_timeToNextEmit += deltaT;
	if (m_timeToNextEmit > (1.0f / getEmissionRate()) && 
		m_particles.size() < size_t(getQuota()))
	{
		m_timeToNextEmit = 0;

		// may emit more than one particle per call
		_emit();
	}

	return true;
}
//---------------------------------------------------------------------------
void Emitter::quiesce()
{
	m_bQuiesced = true;
}
//---------------------------------------------------------------------------
void Emitter::start()
{
	m_bQuiesced = false;
	m_timeToNextEmit = 0; // start emitting immediately
}
//---------------------------------------------------------------------------
void Emitter::getParticlesToRender(ParticleList& list)
{
	// by default, if we've got a particle in the list, it's renderable
	// (derived classes can define their own behavior for this method)
	list = m_particles;
}
//---------------------------------------------------------------------------
void Emitter::appendParticlesToRender(ParticleList& list)
{
	// by default, if we've got a particle in the list, it's renderable
	// (derived classes can define their own behavior for this method)
	list.insert(list.end(), m_particles.begin(), m_particles.end());
}
//---------------------------------------------------------------------------
void Emitter::_emit()
{
}
