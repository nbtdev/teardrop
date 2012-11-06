/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(EMITTER_INCLUDED)
#define EMITTER_INCLUDED

#include "VFX/ParticleData.h"
#include "Game/Component.h"
#include "Math/Vector4.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class Emitter : public Component
	{
	public:
		TD_CLASS(Emitter, Component);
		TD_PROPERTY(Quota, "Max number of particles that this emitter can have active", int, 250, 0);
		TD_PROPERTY(EmissionRate, "Rate of particle emission for this emitter (emits/second)", float, 30, 0);
		TD_PROPERTY(TTLMax, "Maximum Time To Live for particles emitted by this emitter", float, 5, 0);
		TD_PROPERTY(TTLMin, "Minimum Time To Live for particles emitted by this emitter", float, 1, 0);
		TD_PROPERTY(InitVelocityMax, "Maximum initial velocity for particles emitted by this emitter (units/second)", float, 5, 0);
		TD_PROPERTY(InitVelocityMin, "Minimum initial velocity for particles emitted by this emitter (units/second)", float, 1, 0);
		TD_PROPERTY(AccelerationMax, "Maximum acceleration for particles emitted by this emitter - can be negative for deceleration (units/second/second)", float, 0.1f, 0);
		TD_PROPERTY(AccelerationMin, "Minimum acceleration for particles emitted by this emitter - can be negative for deceleration (units/second/second)", float, 0.5f, 0);
		TD_PROPERTY(Duration, "Once started, how long should this emitter run (in seconds), before quiescing? (-1 means 'forever')", float, -1, 0);
		TD_PROPERTY(Delay, "Once quiesced, how long should this emitter idle (in seconds), before restarting? (-1 means 'forever')", float, -1, 0);
		TD_COMPLEX_PROPERTY(Offset, "This emitter's offset in parent particle system space", Vector4, "(0,0,0,0)", 0);

		Emitter();
		~Emitter();

		// Object overrides
		bool initialize();
		bool destroy();

		virtual bool update(float deltaT);
		// halt the emission of new particles; basically, let the current set of 
		// active particles run their course
		virtual void quiesce();
		// start the emitter running again
		virtual void start();

		// so that the emitter can access the system's particle data pool
		void setParticleAllocator(ParticleDataPool* pPool) { m_particleAllocator = pPool; }
		// so that the system can get a list of the currently-visible (and so, 
		// renderable) particles
		virtual void getParticlesToRender(/*out*/ ParticleList& list);
		// same as above, except that behavior is to append particles to the list rather than overwrite it
		virtual void appendParticlesToRender(/*inout*/ ParticleList& list);
		// returns the number of active particles controlled by this emitter
		size_t getActiveParticleCount() { return m_particles.size(); }

		/*
			Derived-class overrides
		*/
		// called when it's time to emit more particles (callee may emit more 
		// than one particle per call, so long as the total number of particles 
		// in play are <= the quota)
		virtual void _emit();

		TD_DECLARE_ALLOCATOR();

	protected:
		ParticleDataPool* m_particleAllocator;
		// a list of the particles we've allocated
		ParticleList m_particles;
		// time until we can emit next particle
		float m_timeToNextEmit;
		// flag to halt emission of new particles
		bool m_bQuiesced;
		// timer for things like 

	private:
		void onInstanceCreated();
	};
}

#endif // EMITTER_INCLUDED
