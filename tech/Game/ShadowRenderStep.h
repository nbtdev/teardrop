/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADOWRENDERSTEP_INCLUDED)
#define SHADOWRENDERSTEP_INCLUDED

#include "RenderStep.h"

namespace Teardrop
{
	class Scene;

	/*
		Render step that renders all relevant dynamic shadowmaps for the 
		scene. 
	*/

	class ShadowRenderStep
		: public RenderStep
	{
	public:
		ShadowRenderStep();
		~ShadowRenderStep();

        void render(const VisibleObjects& objects, Gfx::Renderer* pRenderer, Scene* pScene);

		TD_DECLARE_ALLOCATOR();

	private:
		Scene* m_pScene;
	};
}

#endif // SHADOWRENDERSTEP_INCLUDED
