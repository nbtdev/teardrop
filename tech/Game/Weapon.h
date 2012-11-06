/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
		TD_PROPERTY(Name, "Weapon Name", String, "", 0);
		TD_PROPERTY(Range, "Weapon Range (units)", int, -1, 0);
		TD_PROPERTY(Heat, "Weapon Heat", int, -1, 0);
		TD_PROPERTY(AnimationTime, "Weapon Animation Time (seconds)", float, -1, 0);
		TD_PROPERTY(RecycleTime, "Weapon Recycle Time (seconds)", float, -1, 0);
		TD_PROPERTY(Damage, "Weapon Damage (points)", int, -1, 0);
		TD_PROPERTY(FireSfx_Internal, "Weapon SFX - heard inside an avatar", int, 0, 0);
		TD_PROPERTY(FireSfx_External, "Weapon SFX - heard outside an avatar", int, 0, 0);
		TD_PROPERTY(RecycleSfx, "Weapon SFX - heard while or after a weapons recycles", int, 0, 0);
		TD_PROPERTY(HitSfx, "Weapon SFX - heard when weapon fire hits a target", int, 0, 0);

		Weapon();
		~Weapon();

		//! will only have an effect if the weapon is recycled/reloaded and ready;
		//! firing effect(s)/animation will continue to end once started
		virtual void fire();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // WEAPON_INCLUDED
