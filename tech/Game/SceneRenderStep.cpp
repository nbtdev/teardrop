/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#include "SceneRenderStep.h"
#include "Scene.h"
#include "Zone.h"
#include "ZoneObject.h"
#include "Component_Render.h"
#include "Gfx/Camera.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/RenderQueue.h"
#include "Reflection/Reflection.h"
#include "Reflection/ClassDef.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
SceneRenderStep::SceneRenderStep()
    : m_pVP(nullptr)
{
}
//---------------------------------------------------------------------------
SceneRenderStep::~SceneRenderStep()
{
}
//---------------------------------------------------------------------------
void SceneRenderStep::render(
    const VisibleObjects& objects, Gfx::Renderer* pRend, Scene* pScene)
{
	if (!m_pCamera)
		return;

	// then update/render the main scene
    //pRend->setRenderMode(Gfx::Renderer::RENDER_DEFAULT);
	m_pRT->setCurrent();
//	pRend->clearRenderTarget(); // clears all
//	pRend->setColorWrite(true);

    Gfx::Viewport* vp = m_pVP;
    if (!vp) {
        // use the default viewport on the render target
        vp = m_pRT->viewport();
    }

	m_pCamera->setAspect(m_pRT->aspect());
    pRend->beginScene(m_pCamera, vp);

	// first, find out if there is anything to render
	// get the visible objects from the scene; if any, pass over to the renderer
    Gfx::RenderQueue renderQueue;

	if (objects.size())
	{
		// go through the list and make another of things we need to render
		for(ZoneObjects::const_iterator it = objects.begin();
			it != objects.end(); ++it)
		{
			ZoneObject* pObj = const_cast<ZoneObject*>(*it);
			ZoneObject::ComponentList renderables;
			pObj->findComponents(RenderComponent::getClassDef(), renderables);
            for (auto r : renderables)
			{
				RenderComponent* pRenderable = 
                    static_cast<RenderComponent*>(r);

				// update render component's affecting light list if needed. this
				// call will internally short-circuit if the object is not lit or
				// the light list doesn't need rebuilt
				pRenderable->updateLightList(pScene);
                pRenderable->queueForRendering(&renderQueue);
			}
		}
	}

	pRend->endScene();
}
