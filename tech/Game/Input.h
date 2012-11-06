/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INPUT_INCLUDED)
#define INPUT_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	struct Environment;
	union InputEvent;

	class Input
	{
		// screen size of associated window
		int m_extentX;
		int m_extentY;

	public:
		Input();
		~Input();

		bool initialize(Environment& env, void* hWnd=0);
		bool destroy();

		//! get pending input event into inputEvent, returns false when none pending
		bool getNextEvent(InputEvent& /*out*/ inputEvent);

		/** Accessors
		*/
		void getExtents(int& x, int& y);

		/** Mutators
		*/
		void setExtents(int x, int y);
		void suspend(); // stop capturing input events
		void resume(); // restart capturing input events

		TD_DECLARE_ALLOCATOR();
		struct Impl;

	private:
		Impl* m_pImpl;
	};
}

#endif // INPUT_INCLUDED
