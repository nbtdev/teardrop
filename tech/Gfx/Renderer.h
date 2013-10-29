/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERER_INCLUDED)
#define RENDERER_INCLUDED

#include "Gfx/Common.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class RenderTarget;
		class Texture2D;

		class Renderer
		{
		public:
			Renderer();
			virtual ~Renderer() = 0;

			enum Flags {
				INIT_FRAMEBUFFER_ALPHA = (1<<0),
				INIT_ENABLE_VSYNC = (1<<1),
				INIT_ENABLE_STENCIL_BUFFER = (1<<2),
				INIT_ENABLE_DEPTH_BUFFER = (1<<3),
				INIT_FULLSCREEN = (1<<4),
				INIT_ENABLE_NVPERFHUD = (1<<5),
			};

			// initialize renderer with main/default render target (typically a render
			// window); flags is a bitwise OR of the Flags enumeration values; depending 
			// on platform, an hWnd value of 0 means either "create a new top level window"
			// or "use the window/context created for me"
			virtual RenderTarget* initialize(uintptr_t hWnd, int flags) = 0;
			virtual void shutdown() = 0;

			// create a new render window; if hWnd is 0, creates a new top-level
			// window, otherwise, creates an embedded render context in the supplied 
			// window; 
			virtual RenderTarget* createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags) = 0;
			// create a new render texture; if tex is null, no render texture is created
			virtual RenderTarget* createRenderTexture(int w, int h, SurfaceFormat fmt, int flags) = 0;
			// set render target as current render target
			virtual void setRenderTarget(RenderTarget* rt) = 0;

			// begin a new frame render
			virtual void beginFrame(
				bool color = true,
				unsigned int clearColor = 0,
				bool depth = true,
				float depthValue = 1,
				bool stencil = true,
				unsigned int stencilValue = 0) = 0;
			// begin a new scene (frame subset)
			virtual void beginScene() = 0;
			// end scene
			virtual void endScene() = 0;
			// end frame render and swap buffers (if applicable)
			virtual void endFrame() = 0;
		};
	}
}

#endif // RENDERER_INCLUDED
