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

#if !defined(WEAPON_INCLUDED)
#define WEAPON_INCLUDED

#include "Game/Mountable.h"
#include "Reflection/Property.h"

namespace Teardrop
{
	namespace Audio
	{
		class Sample;
	}

	class Weapon
		: public Mountable
	{
	public:
		TD_CLASS(Weapon, Mountable);
		TD_PROPERTY(Range, "Weapon Range (units)", int, -1, 0);
		TD_PROPERTY(Heat, "Weapon Heat", int, -1, 0);
		TD_PROPERTY(AnimationTime, "Weapon Animation Time (seconds)", float, -1, 0);
		TD_PROPERTY(RecycleTime, "Weapon Recycle Time (seconds)", float, -1, 0);
		TD_PROPERTY(Damage, "Weapon Damage (points)", int, -1, 0);
		TD_PROPERTY(FireSfx_Internal, "Weapon SFX - heard inside an avatar", int, 0, 0);
		TD_PROPERTY(FireSfx_External, "Weapon SFX - heard outside an avatar", int, 0, 0);
		TD_PROPERTY(RecycleSfx, "Weapon SFX - heard while or after a weapons recycles", int, 0, 0);
		TD_PROPERTY(HitSfx, "Weapon SFX - heard when weapon fire hits a target", int, 0, 0);
		TD_PROPERTY(Size, "Size of weapon in slot (0 or greater)", int, 0, 0);

		Weapon();
		~Weapon();

		//! will only have an effect if the weapon is recycled/reloaded and ready;
		//! firing effect(s)/animation will continue to end once started
		virtual void fire();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // WEAPON_INCLUDED
