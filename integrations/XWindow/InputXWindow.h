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

#if !defined(INPUTXWINDOW_INCLUDED)
#define INPUTXWINDOW_INCLUDED

#include "Core/Input.h"
#include "Memory/Allocators.h"
#include <X11/Xlib.h>

namespace Teardrop
{
    namespace XWindow
    {
        class Input : public Teardrop::Input
        {
        public:
            Input(Display* aDisplay);
            ~Input();

            void initialize(uintptr_t aWindow);
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
            int mExtentX = 0;
            int mExtentY = 0;

            struct AbsPos
            {
                int x, y, z;
            };

            Display* mDisplay = nullptr;
            Window mWindow = 0;
            bool mLCtrl = false;
            bool mRCtrl = false;
            bool mLAlt = false;
            bool mRAlt = false;
            bool mLShift = false;
            bool mRShift = false;
        };
    } // namespace XWindow
} // namespace Teardrop

#endif // INPUTXWINDOW_INCLUDED
