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
