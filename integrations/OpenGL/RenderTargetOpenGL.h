/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERTARGETOPENGL_INCLUDED)
#define RENDERTARGETOPENGL_INCLUDED

#include "GLHeaders.h"
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
                    unsigned int stencilValue = 0) override;
                float aspect() override;
                int width() override;
                int height() override;
                Gfx::Viewport* addViewport(float x = 0, float y = 0, float w = 1, float h = 1, size_t zOrder = 0) override;
                size_t viewportCount(size_t zOrder) const override;
                Gfx::Viewport* viewport(size_t index = 0, size_t zOrder = 0) const override;
                void releaseViewport(Viewport* vp);

                TD_DECLARE_ALLOCATOR();

            protected:
                int mWidth = 0;
                int mHeight = 0;

                typedef std::multimap<size_t /*zOrder*/, Viewport*> Viewports;
                Viewports mViewports;
            };
        }
    }
}

#endif // RENDERTARGETOPENGL_INCLUDED
