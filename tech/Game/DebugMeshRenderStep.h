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

#if !defined(DEBUGMESHRENDERSTEP_INCLUDED)
#define DEBUGMESHRENDERSTEP_INCLUDED

#include "RenderStep.h"

namespace Teardrop
{
	/*
		Render step that renders the physics debug meshes
	*/

    namespace Gfx {
        class Camera;
        class Viewport;
        class RenderTarget;
    }

	class DebugMeshRenderStep
		: public RenderStep
	{
        Gfx::Camera* m_pCam;
        Gfx::Viewport* m_pVP;
        Gfx::RenderTarget* m_pRT;
		bool m_bEnabled;

	public:
		DebugMeshRenderStep();
		~DebugMeshRenderStep();

		void enable(bool bEnable) { m_bEnabled = bEnable; }
		bool isEnabled() { return m_bEnabled; }
		void toggleEnabled() { m_bEnabled = !m_bEnabled; }

        void setCamera(Gfx::Camera* pCam) { m_pCam = pCam; }
        void setViewport(Gfx::Viewport* pVP) { m_pVP = pVP; }
        void setRenderTarget(Gfx::RenderTarget* pRT) { m_pRT = pRT; }

		// RenderStep implementation
        void render(const VisibleObjects& objects, Gfx::Renderer* pRenderer, Scene* pScene);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // DEBUGMESHRENDERSTEP_INCLUDED
