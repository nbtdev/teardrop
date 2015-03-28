/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "Gfx/Renderer.h"
#include "Memory/Allocators.h"
#include <vector>

namespace Teardrop
{
    namespace Gfx
    {
        namespace OpenGL
        {
            class Renderer
                : public Gfx::Renderer
            {
            public:
                Renderer();
                ~Renderer();

                // initialize renderer with main/default render target (typically a render
                // window); flags is a bitwise OR of the Flags enumeration values; depending
                // on platform, an hWnd value of 0 means either "create a new top level window"
                // or "use the window/context created for me"
                Gfx::RenderTarget* initialize(uintptr_t hWnd, int flags);
                void shutdown();

                // create a new render window; if hWnd is 0, creates a new top-level
                // window, otherwise, creates an embedded render context in the supplied
                // window;
                Gfx::RenderTarget* createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags);
                // create a new render texture; if tex is null, no render texture is created
                Gfx::RenderTarget* createRenderTexture(int w, int h, SurfaceFormat fmt, int flags);
                // release a previously-created render target
                void releaseRenderTarget(Gfx::RenderTarget* rt);
                // set render target as current render target
                void setRenderTarget(Gfx::RenderTarget* rt);

                // begin a new frame render
                void beginFrame(
                    bool color = true,
                    unsigned int clearColor = 0,
                    bool depth = true,
                    float depthValue = 1,
                    bool stencil = true,
                    unsigned int stencilValue = 0);
                // begin a new scene (frame subset)
                void beginScene(Camera* camera, Viewport* vp=0);
                // begin rendering a new object (mesh instance)
                void beginObject(const Matrix44& worldXf);
                // set material (shader) used for subsequent renders
                void apply(Material* material);
                // render
                void render(Submesh* submesh);
                // end object (mesh instance)
                void endObject();
                // end scene
                void endScene();
                // end frame render and swap buffers (if applicable)
                void endFrame();

                TD_DECLARE_ALLOCATOR();

            private:
                typedef std::vector<Gfx::RenderTarget*> RenderTargets;
                RenderTargets mRenderTargets;

                RenderTarget* mCurrentRT = nullptr;
            };
        }
    }
}

#endif // OPENGLRENDERER_H
