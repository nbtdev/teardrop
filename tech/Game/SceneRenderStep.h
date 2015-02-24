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
