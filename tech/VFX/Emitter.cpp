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

#include "Emitter.h"

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(Emitter);
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
