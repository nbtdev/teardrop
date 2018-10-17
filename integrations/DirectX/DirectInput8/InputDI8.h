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

#if !defined(INPUTDI8_INCLUDED)
#define INPUTDI8_INCLUDED

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>

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
