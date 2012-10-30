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

#if !defined(EMITTER_INCLUDED)
#define EMITTER_INCLUDED

#include "VFX/include/ParticleData.h"
#include "Game/include/Component.h"
#include "Math/include/Vector4.h"
#include "Memory/include/Allocators.h"

namespace CoS
{
	class Emitter : public Component
	{
	public:
		COS_CLASS(Emitter, Component);
		COS_PROPERTY(Quota, "Max number of particles that this emitter can have active", int, 250, 0);
		COS_PROPERTY(EmissionRate, "Rate of particle emission for this emitter (emits/second)", float, 30, 0);
		COS_PROPERTY(TTLMax, "Maximum Time To Live for particles emitted by this emitter", float, 5, 0);
		COS_PROPERTY(TTLMin, "Minimum Time To Live for particles emitted by this emitter", float, 1, 0);
		COS_PROPERTY(InitVelocityMax, "Maximum initial velocity for particles emitted by this emitter (units/second)", float, 5, 0);
		COS_PROPERTY(InitVelocityMin, "Minimum initial velocity for particles emitted by this emitter (units/second)", float, 1, 0);
		COS_PROPERTY(AccelerationMax, "Maximum acceleration for particles emitted by this emitter - can be negative for deceleration (units/second/second)", float, 0.1f, 0);
		COS_PROPERTY(AccelerationMin, "Minimum acceleration for particles emitted by this emitter - can be negative for deceleration (units/second/second)", float, 0.5f, 0);
		COS_PROPERTY(Duration, "Once started, how long should this emitter run (in seconds), before quiescing? (-1 means 'forever')", float, -1, 0);
		COS_PROPERTY(Delay, "Once quiesced, how long should this emitter idle (in seconds), before restarting? (-1 means 'forever')", float, -1, 0);
		COS_COMPLEX_PROPERTY(Offset, "This emitter's offset in parent particle system space", Vector4, "(0,0,0,0)", 0);

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

		COS_DECLARE_ALLOCATOR();

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
