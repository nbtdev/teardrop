/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERTARGETOPENGL_INCLUDED)
#define RENDERTARGETOPENGL_INCLUDED

#include "Gfx/RenderTarget.h"
#include "Memory/Allocators.h"
#include <map>

namespace Teardrop
{
    namespace Gfx
    {
        namespace OpenGL
        {
            class RenderTarget : public Gfx::RenderTarget
            {
            public:
                RenderTarget();
                ~RenderTarget();

                void clear(
                    bool color = true,
                    unsigned int clearColor = 0,
                    bool depth = true,
                    float depthValue = 1,
                    bool stencil = true,
                    unsigned int stencilValue = 0);
                int width();
                int height();
                Viewport* addViewport(float x=0, float y=0, float w=1, float h=1, unsigned int zOrder=0);
                void releaseViewport(Viewport* vp);

                TD_DECLARE_ALLOCATOR();

            protected:
                int mWidth = 0;
                int mHeight = 0;

                typedef std::multimap<unsigned int /*zOrder*/, Viewport*> Viewports;
                Viewports mViewports;
            };
        }
    }
}

#endif // RENDERTARGETOPENGL_INCLUDED
