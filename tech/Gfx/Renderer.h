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

#if !defined(RENDERER_INCLUDED)
#define RENDERER_INCLUDED

#include "Gfx/Common.h"
#include <cstdlib>
#include <cstdint>
#include <memory>

namespace Teardrop
{
	class Matrix44;

	namespace Gfx 
	{
		class RenderTarget;
		class Texture2D;
		class Camera;
		class Viewport;
		class Material;
        class Pipeline;
		class Submesh;

		class Renderer
		{
		public:
			virtual ~Renderer();

			// create a new render window; if hWnd is 0, creates a new top-level
			// window, otherwise, creates an embedded render context in the supplied 
			// window; 
			virtual std::shared_ptr<RenderTarget> createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags) = 0;
			// create a new render texture; if tex is null, no render texture is created
			virtual std::shared_ptr<RenderTarget> createRenderTexture(int w, int h, SurfaceFormat fmt, int flags) = 0;

			// begin a new frame render
			virtual void beginFrame() = 0;
			// begin a new scene (frame subset)
			virtual void beginScene(Camera* camera, Viewport* vp=0) = 0;
			// begin rendering a new object (mesh instance)
			virtual void beginObject(const Matrix44& worldXf) = 0;
            // set up the graphics pipeline to render one or more Renderables
            virtual void apply(Pipeline* pipeline) = 0;
            // render
			virtual void render(Submesh* submesh) = 0;
			// end object (mesh instance)
			virtual void endObject() = 0;
			// end scene
			virtual void endScene() = 0;
			// end frame render and swap buffers (if applicable)
			virtual void endFrame() = 0;
		};
	}
}

#endif // RENDERER_INCLUDED
