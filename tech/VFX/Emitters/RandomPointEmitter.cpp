/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RandomPointEmitter.h"
#include "Math/MathUtil.h"
#include "Math/Quaternion.h"
#include <limits>

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(RandomPointEmitter);
//---------------------------------------------------------------------------
RandomPointEmitter::RandomPointEmitter()
{
}
//---------------------------------------------------------------------------
RandomPointEmitter::~RandomPointEmitter()
{

}
//---------------------------------------------------------------------------
bool RandomPointEmitter::update(float deltaT)
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
void RandomPointEmitter::_emit()
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
	unsigned long lAng1 = MathUtil::mt_rand();
	unsigned long lAng2 = MathUtil::mt_rand();
	unsigned long minVal = std::numeric_limits<unsigned long>::min();
	unsigned long maxVal = std::numeric_limits<unsigned long>::max();
	float vel = (float(minVal) + float(lVel) / float(maxVal)) * 
		(getInitVelocityMax() - getInitVelocityMin()) + getInitVelocityMin();
	float acc = (float(minVal) + float(lAcc) / float(maxVal)) *
		(getAccelerationMax() - getAccelerationMin()) + getAccelerationMin();
	float ttl = (float(minVal) + float(lTTL) / float(maxVal)) *
		(getTTLMax() - getTTLMin()) + getTTLMin();
	float ang1 = (float(minVal) + float(lAng1) / float(maxVal)) *
		(getDeviation() - (-getDeviation())) + getDeviation();
	float ang2 = (float(minVal) + float(lAng2) / float(maxVal)) *
		(getDeviation() - (-getDeviation())) + (-getDeviation());

	Quaternion q(ang1, Vector4::UNIT_X);
	Vector4 randDir(q * getDirection());
	q.fromAngleAxis(ang2, Vector4::UNIT_Z);
	randDir += (q * getDirection());

	pParticle->m_velocity = q * randDir * vel;
	pParticle->m_acceleration = q * randDir * acc;
	pParticle->m_position = getOffset();
	// TODO: scale can be animated
	pParticle->m_scale = 1;
	// initialize particle lifetimer
	pParticle->m_time = 0;
	pParticle->m_ttl = ttl;
}
