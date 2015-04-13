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
