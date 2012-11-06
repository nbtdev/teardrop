/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SCENERENDERSTEP_INCLUDED)
#define SCENERENDERSTEP_INCLUDED

#include "RenderStep.h"

namespace Teardrop
{
	/*
		Render step that renders the normal 3D perspective view 
		of the scene
	*/

	class GfxViewport;
	class GfxRenderTarget;
	class Scene;

	class SceneRenderStep
		: public RenderStep
	{
		GfxViewport* m_pVP;
		GfxRenderTarget* m_pRT;
		Scene* m_pScene;

	public:
		SceneRenderStep();
		~SceneRenderStep();

		void setViewport(GfxViewport* pVP) { m_pVP = pVP; }
		void setRenderTarget(GfxRenderTarget* pRT) { m_pRT = pRT; }

		// RenderStep implementation
		void render(const VisibleObjects& objects, GfxRenderer* pRenderer, Scene* pScene);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SCENERENDERSTEP_INCLUDED
