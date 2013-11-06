/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INPUT_INCLUDED)
#define INPUT_INCLUDED

#include "Util/Singleton.h"

namespace Teardrop
{
	union InputEvent;

	class Input : public Singleton<Input>
	{
	public:
		Input();
		virtual ~Input();

		//! get pending input event into inputEvent, returns false when none pending
		virtual bool getNextEvent(InputEvent& /*out*/ inputEvent) = 0;
		virtual void suspend() = 0; // stop capturing input events
		virtual void resume() = 0; // restart capturing input events
	};
}

#endif // INPUT_INCLUDED
