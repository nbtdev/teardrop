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
