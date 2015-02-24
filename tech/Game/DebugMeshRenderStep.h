/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
