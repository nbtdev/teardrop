/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERWINDOWOPENGL_INCLUDED)
#define RENDERWINDOWOPENGL_INCLUDED

#include "RenderTargetOpenGL.h"
#include <X11/Xlib.h>
#include <GL/glx.h>

namespace Teardrop
{
    namespace Gfx
    {
        namespace OpenGL
        {

            class RenderWindow : public RenderTarget
            {
            public:
                RenderWindow(Display* aDisplay);
                RenderWindow(Display* aDisplay, Window aParent);
                RenderWindow(Display* aDisplay, int aFlags);
                RenderWindow(Display* aDisplay, Window aParent, int aFlags);
                ~RenderWindow();

                void present();
                void resize(int w, int h);
                void setCurrent();

                TD_DECLARE_ALLOCATOR();

            protected:
                int mInitFlags = 0;
                Display* mDisplay = nullptr;
                Window mParent = 0;
                Window mWindow = 0;
                GLXContext mCtx = nullptr;
            };

        } // namespace OpenGL
    } // namespace Gfx
} // namespace Teardrop

#endif // RENDERWINDOWOPENGL_INCLUDED
