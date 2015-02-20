/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(POINTEMITTER_INCLUDED)
#define POINTEMITTER_INCLUDED

#include "VFX/Emitter.h"

namespace Teardrop
{
	// a point emitter emits particles at a specific point; particles are not 
	// imparted any velocity or direction; an affector is needed to make particles
	// move away from a point emitter
	class PointEmitter : public Emitter
	{
	public:
		TD_CLASS(PointEmitter, Emitter);
		TD_CLASS_CREATABLE();
		TD_COMPLEX_PROPERTY(Direction, "Nominal direction (center) of emitted particles", Vector4, "(0,1,0,0)", 0);

		PointEmitter();
		~PointEmitter();

		bool initialize();
		bool destroy();

		bool update(float deltaT);

		/*
			Emitter overrides
		*/

		// emits one particle
		void _emit();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // EMITTER_INCLUDED
