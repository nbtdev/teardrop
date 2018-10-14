/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(BEAMWEAPON_INCLUDED)
#define BEAMWEAPON_INCLUDED

#include "Game/Weapon.h"

namespace Teardrop
{
    namespace Gfx {
        class ShaderConstant;
    }

	class CountdownTrigger;

	class BeamWeapon
		: public Weapon
	{
        Gfx::ShaderConstant* m_pMountTransformConstant;

	public:
		TD_CLASS(BeamWeapon, Weapon);

		BeamWeapon();
		~BeamWeapon();

		// Component overrides
		bool _initialize();
		bool _destroy();
		bool _update(float deltaT);

		// Weapon implementation
		void fire();

		// Trigger callback(s)
		void playRecycleSfx();

		TD_DECLARE_ALLOCATOR();

	private:

		struct Vertex
		{
			float px, py, pz;
			float tx, ty;
			TD_DECLARE_ALLOCATOR();
		};

		Vertex* m_pVertexData;

		// firing control
		float m_animationTimeRemaining;
		float m_recycleTimeRemaining;
	};
}

#endif // BEAMWEAPON_INCLUDED
