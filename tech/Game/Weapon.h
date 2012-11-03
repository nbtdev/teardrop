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

#if !defined(WEAPON_INCLUDED)
#define WEAPON_INCLUDED

#include "Game/Mountable.h"
#include "Reflection/Property.h"

namespace CoS
{
	namespace Audio
	{
		class Sample;
	}

	class Weapon
		: public Mountable
	{
	public:
		COS_CLASS(Weapon, Mountable);
		COS_PROPERTY(Name, "Weapon Name", String, "", 0);
		COS_PROPERTY(Range, "Weapon Range (units)", int, -1, 0);
		COS_PROPERTY(Heat, "Weapon Heat", int, -1, 0);
		COS_PROPERTY(AnimationTime, "Weapon Animation Time (seconds)", float, -1, 0);
		COS_PROPERTY(RecycleTime, "Weapon Recycle Time (seconds)", float, -1, 0);
		COS_PROPERTY(Damage, "Weapon Damage (points)", int, -1, 0);
		COS_PROPERTY(FireSfx_Internal, "Weapon SFX - heard inside an avatar", int, 0, 0);
		COS_PROPERTY(FireSfx_External, "Weapon SFX - heard outside an avatar", int, 0, 0);
		COS_PROPERTY(RecycleSfx, "Weapon SFX - heard while or after a weapons recycles", int, 0, 0);
		COS_PROPERTY(HitSfx, "Weapon SFX - heard when weapon fire hits a target", int, 0, 0);

		Weapon();
		~Weapon();

		//! will only have an effect if the weapon is recycled/reloaded and ready;
		//! firing effect(s)/animation will continue to end once started
		virtual void fire();

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // WEAPON_INCLUDED
