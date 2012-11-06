/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RANDOMPOINTEMITTER_INCLUDED)
#define RANDOMPOINTEMITTER_INCLUDED

#include "PointEmitter.h"

namespace Teardrop
{
	// a point emitter emits particles at a specific point; particles are not 
	// imparted any velocity or direction; an affector is needed to make particles
	// move away from a point emitter
	class RandomPointEmitter : public PointEmitter
	{
	public:
		TD_CLASS(RandomPointEmitter, PointEmitter);
		TD_CLASS_CREATABLE();
		TD_PROPERTY(Deviation, "Maximum deviation (in degrees) from nominal direction of emitted particles", float, 30, 0);

		RandomPointEmitter();
		~RandomPointEmitter();

		// ZoneObject override
		bool update(float deltaT);

		/*
			Emitter overrides
		*/

		// emits one particle
		void _emit();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // RANDOMPOINTEMITTER_INCLUDED
