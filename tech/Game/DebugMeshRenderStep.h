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

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // DEBUGMESHRENDERSTEP_INCLUDED
