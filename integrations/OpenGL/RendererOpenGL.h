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
#include <map>
#include <memory>
#include <vector>

namespace Teardrop
{
    namespace Gfx
    {
		class Material;

        namespace OpenGL
        {
			class Program;

            class Renderer
                : public Gfx::Renderer
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
                typedef std::vector<std::shared_ptr<Gfx::RenderTarget>> RenderTargets;
                RenderTargets mRenderTargets;

				// the render sequence is such that material changes are made before the submeshes
				// that the material shades are provided, so since GL uses programs instead of 
				// applying shaders individually, we need to track the current material so that 
				// we can pull the correct program for the fragment+vertex shaders as needed
				Material* mCurrentMtl = nullptr;

				// somewhere to track GLSL programs based on submesh and material combination
				typedef std::map<uint64_t, std::shared_ptr<Program>> Programs;
				Programs mPrograms;

				std::shared_ptr<Program> findOrCreateProgram(Material* aMaterial, Submesh* aSubmesh);

				// avoid changing state unnecessarily (GL probably prevents this anyway...)
				std::shared_ptr<Program> mCurrentProgram;

				std::shared_ptr<Gfx::RenderTarget> createDummyContext();
            };
        }
    }
}

#endif // OPENGLRENDERER_H
