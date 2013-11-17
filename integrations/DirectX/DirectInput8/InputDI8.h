/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INPUTDI8_INCLUDED)
#define INPUTDI8_INCLUDED

#include "Core/Input.h"
#include "Memory/Allocators.h"
#include <vector>

namespace Teardrop
{
	namespace DirectInput 
	{
		class Input : public Teardrop::Input
		{
		public:
			Input();
			~Input();

			void initialize(uintptr_t hWnd=0);
			void destroy();

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

		private:
			// screen size of associated window
			int mExtentX;
			int mExtentY;

			struct AbsPos
			{
				int x, y, z;
			};

			IDirectInput8* mDI;
			std::vector<IDirectInputDevice8*> mMouse;
			IDirectInputDevice8* mKeyboard;
			IDirectInputDevice8* mStick;
			std::vector<AbsPos> mMousePos;
			HWND mHwnd;
			bool mLCtrl;
			bool mRCtrl;
			bool mLAlt;
			bool mRAlt;
			bool mLShift;
			bool mRShift;
		};
	}
}

#endif // INPUTDI8_INCLUDED
