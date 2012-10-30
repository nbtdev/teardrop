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

#include "PointEmitter.h"
#include "Math/include/MathUtil.h"
#include <limits>

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(PointEmitter);
//---------------------------------------------------------------------------
PointEmitter::PointEmitter()
{
}
//---------------------------------------------------------------------------
PointEmitter::~PointEmitter()
{

}
//---------------------------------------------------------------------------
bool PointEmitter::initialize()
{
	return Emitter::initialize();
}
//---------------------------------------------------------------------------
bool PointEmitter::destroy()
{
	return Emitter::destroy();
}
//---------------------------------------------------------------------------
bool PointEmitter::update(float deltaT)
{
	// invoke the base class update (performs common tasks such as emission
	// rate control)
	Emitter::update(deltaT);

	// then update all of the particles we have in play
	for (ParticleList::iterator it = m_particles.begin(); 
		it != m_particles.end(); ++it)
	{
		ParticleData* pParticle = *it;

		// simple second-order Newtonian movement -- update the particle 
		// positions based on 
		//   p = p0 + v0 * t + 1/2 * a * t^2
		//   v = v0 + a * t
		// until the particle expires, anyway...at which point we remove
		// it from our list and "put it back in the box"
		pParticle->m_time += deltaT;
		if (pParticle->m_time > pParticle->m_ttl)
		{
			// remove from list and deallocate particle
			m_particleAllocator->deallocate(pParticle);
			it = m_particles.erase(it);
			if (it == m_particles.end())
				return true;
			else
				continue;
		}

		// otherwise, update the velocity and position (in particle system space)
		pParticle->m_velocity = pParticle->m_velocity + 
			pParticle->m_acceleration * deltaT;
		pParticle->m_position = pParticle->m_position + 
			pParticle->m_velocity * deltaT + 
			0.5f * pParticle->m_acceleration * deltaT * deltaT;
	}

	return true;
}
//---------------------------------------------------------------------------
void PointEmitter::_emit()
{
	// emit another particle along the direction vector, at some velocity 
	// between InitVelocityMax and InitVelocityMin
	ParticleData* pParticle = m_particleAllocator->allocate();
	if (!pParticle)
		return; // should not happen?

	m_particles.push_back(pParticle);

	unsigned long lVel = MathUtil::mt_rand();
	unsigned long lAcc = MathUtil::mt_rand();
	unsigned long lTTL = MathUtil::mt_rand();
	unsigned long minVal = std::numeric_limits<unsigned long>::min();
	unsigned long maxVal = std::numeric_limits<unsigned long>::max();
	float vel = (float(minVal) + float(lVel) / float(maxVal)) * 
		(getInitVelocityMax() - getInitVelocityMin()) + getInitVelocityMin();
	float acc = (float(minVal) + float(lAcc) / float(maxVal)) *
		(getAccelerationMax() - getAccelerationMin()) + getAccelerationMin();
	float ttl = (float(minVal) + float(lTTL) / float(maxVal)) *
		(getTTLMax() - getTTLMin()) + getTTLMin();

	pParticle->m_velocity = getDirection() * vel;
	pParticle->m_acceleration = getDirection() * acc;
	pParticle->m_position = getOffset();
	// TODO: scale can be animated
	pParticle->m_scale = 1;
	// initialize particle lifetimer
	pParticle->m_time = 0;
	pParticle->m_ttl = ttl;
}
