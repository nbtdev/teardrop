/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VIEWPORTOPENGL_INCLUDED)
#define VIEWPORTOPENGL_INCLUDED

#include "Gfx/Viewport.h"

namespace Teardrop
{
    namespace Gfx {
        namespace OpenGL {
            class Viewport : public Gfx::Viewport
            {
            public:
                Viewport(Gfx::RenderTarget* rt);
                ~Viewport();

                TD_DECLARE_ALLOCATOR();

            protected:
            };
        }
    }
}

#endif // VIEWPORTOPENGL_INCLUDED
