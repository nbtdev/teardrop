/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(SCENERENDERSTEP_INCLUDED)
#define SCENERENDERSTEP_INCLUDED

#include "RenderStep.h"

namespace Teardrop
{
	/*
		Render step that renders the normal 3D perspective view 
		of the scene
	*/

    namespace Gfx {
        class Viewport;
        class RenderTarget;
    }

	class Scene;

	class SceneRenderStep
		: public RenderStep
	{
        Gfx::Viewport* m_pVP;
        Gfx::RenderTarget* m_pRT;
		Scene* m_pScene;

	public:
		SceneRenderStep();
		~SceneRenderStep();

        void setViewport(Gfx::Viewport* pVP) { m_pVP = pVP; }
        void setRenderTarget(Gfx::RenderTarget* pRT) { m_pRT = pRT; }

		// RenderStep implementation
        void render(const VisibleObjects& objects, Gfx::Renderer* pRenderer, Scene* pScene);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SCENERENDERSTEP_INCLUDED
