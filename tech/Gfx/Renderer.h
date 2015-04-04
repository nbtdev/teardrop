/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
			// set material (shader) used for subsequent renders
			virtual void apply(Material* material) = 0;
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
