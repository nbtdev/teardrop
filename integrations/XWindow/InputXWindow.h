/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
