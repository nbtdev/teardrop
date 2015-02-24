/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "SceneRenderer.h"
#include "RenderStep.h"
#include "Scene.h"
#include "Zone.h"
#include "ZoneObject.h"
#include "Component_Render.h"
#include "Gfx/Renderer.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
SceneRenderer::SceneRenderer()
{
	m_pCamera = 0;
}
//---------------------------------------------------------------------------
SceneRenderer::~SceneRenderer()
{
	for (size_t i=0; i<m_renderSteps.size(); ++i)
	{
		delete m_renderSteps[i];
	}
}
//---------------------------------------------------------------------------
void SceneRenderer::addStep(RenderStep* pStep)
{
	m_renderSteps.push_back(pStep);
}
//---------------------------------------------------------------------------
void SceneRenderer::render(
	const ZoneObjects& visibleObjects, 
    Gfx::Renderer* pRenderer,
	Scene* pScene,
    Gfx::Camera* pViewCam)
{
	if (!pRenderer)
		return;

	pRenderer->beginFrame(false, 0, false, 0, false, 0); // let the steps clear as needed

	for (size_t i=0; i<m_renderSteps.size(); ++i)
	{
		if (m_renderSteps[i])
		{
			if (pViewCam)
				m_renderSteps[i]->setCamera(pViewCam);
			else
				m_renderSteps[i]->setCamera(m_pCamera);

			m_renderSteps[i]->render(visibleObjects, pRenderer, pScene);
		}
	}

	pRenderer->endFrame();
}
