/******************************************************************************
Copyright (c) 2019 Teardrop Games

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

#pragma once

#include "Gfx/Renderer.h"
#include "Memory/Allocators.h"

#if defined(WIN32)
#define VK_USE_PLATFORM_WIN32_KHR 1
#else // WIN32
#define VK_USE_PLATFORM_XCB_KHR 1
#define VK_USE_PLATFORM_XLIB_KHR 1
//#define VK_USE_PLATFORM_XLIB_XRANDR_EXT 1
//#define VK_USE_PLATFORM_WAYLAND_KHR 1
#endif // WIN32

#include <vulkan/vulkan.h>

#include <map>
#include <memory>
#include <vector>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

class Renderer : public Gfx::Renderer
{
public:
    Renderer(int flags);
    ~Renderer();

    // create a new render window; if hWnd is 0, creates a new top-level
    // window, otherwise, creates an embedded render context in the supplied
    // window;
    std::shared_ptr<Gfx::RenderTarget> createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags);
    // create a new render texture; if tex is null, no render texture is created
    std::shared_ptr<Gfx::RenderTarget> createRenderTexture(int w, int h, SurfaceFormat fmt, int flags);

    // begin a new frame render
    void beginFrame();
    // begin a new scene (frame subset)
    void beginScene(Camera* camera, Viewport* vp=nullptr);
    // begin rendering a new object (mesh instance)
    void beginObject(const Matrix44& worldXf);
    // set up pipeline used for subsequent renders
    void apply(Pipeline* pipeline);
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
    typedef std::vector<std::shared_ptr<Gfx::RenderTarget>> RenderTargets;
    RenderTargets mRenderTargets;

    VkInstance mInstance;
    VkDevice mDevice;
};

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
