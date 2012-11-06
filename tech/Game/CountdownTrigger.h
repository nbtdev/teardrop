/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COUNTDOWNTRIGGER_INCLUDED)
#define COUNTDOWNTRIGGER_INCLUDED

#include "Game/Trigger.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	/*
		Implementation of Trigger that fires once after a set amount of time. 
		After the trigger fires, it sets its countdown time to positive 
		infinity, unless "repeat" is set to true, in which case it will fire
		every 'secondsUntilFire' seconds until disabled or deleted.
	*/
	class CountdownTrigger
		: public Trigger
	{
	public:
		CountdownTrigger(float secondsUntilFire, bool repeat=false);
		~CountdownTrigger();

		bool update(float deltaT);
		void reset(float secondsUntilFire=-1);

		TD_DECLARE_ALLOCATOR();

	protected:
		float m_secondsUntilFire;
		float m_countdownTimer;
		bool m_repeat;
	};
}

#endif // COUNTDOWNTRIGGER_INCLUDED
