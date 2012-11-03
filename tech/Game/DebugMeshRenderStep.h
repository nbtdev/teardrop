/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(DEBUGMESHRENDERSTEP_INCLUDED)
#define DEBUGMESHRENDERSTEP_INCLUDED

#include "RenderStep.h"

namespace CoS
{
	/*
		Render step that renders the physics debug meshes
	*/

	class GfxCamera;
	class GfxViewport;
	class GfxRenderTarget;

	class DebugMeshRenderStep
		: public RenderStep
	{
		GfxCamera* m_pCam;
		GfxViewport* m_pVP;
		GfxRenderTarget* m_pRT;
		bool m_bEnabled;

	public:
		DebugMeshRenderStep();
		~DebugMeshRenderStep();

		void enable(bool bEnable) { m_bEnabled = bEnable; }
		bool isEnabled() { return m_bEnabled; }
		void toggleEnabled() { m_bEnabled = !m_bEnabled; }

		void setCamera(GfxCamera* pCam) { m_pCam = pCam; }
		void setViewport(GfxViewport* pVP) { m_pVP = pVP; }
		void setRenderTarget(GfxRenderTarget* pRT) { m_pRT = pRT; }

		// RenderStep implementation
		void render(const VisibleObjects& objects, GfxRenderer* pRenderer, Scene* pScene);

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // DEBUGMESHRENDERSTEP_INCLUDED
