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

#include "SceneRenderStep.h"
#include "Scene.h"
#include "Zone.h"
#include "ZoneObject.h"
#include "Component_Render.h"
#include "Gfx/include/GfxRenderer.h"
#include "Gfx/include/GfxRenderTarget.h"
#include "Reflection/include/Reflection.h"
#include "Reflection/include/ClassDef.h"

using namespace CoS;
//---------------------------------------------------------------------------
SceneRenderStep::SceneRenderStep()
{
}
//---------------------------------------------------------------------------
SceneRenderStep::~SceneRenderStep()
{
}
//---------------------------------------------------------------------------
void SceneRenderStep::render(
	const VisibleObjects& objects, GfxRenderer* pRend, Scene* pScene)
{
	if (!m_pCamera)
		return;

	// then update/render the main scene
	pRend->setRenderMode(GfxRenderer::RENDER_DEFAULT);
	pRend->setRenderTarget(m_pRT);
	pRend->clearRenderTarget(); // clears all 
	pRend->setColorWrite(true);
	pRend->beginScene(m_pCamera, m_pVP);

	// first, find out if there is anything to render
	// get the visible objects from the scene; if any, pass over to the renderer
	if (objects.size())
	{
		// go through the list and make another of things we need to render
		for(ZoneObjects::const_iterator it = objects.begin();
			it != objects.end(); ++it)
		{
			ZoneObject* pObj = const_cast<ZoneObject*>(*it);
			ZoneObject::ComponentList renderables;
			pObj->findComponents(RenderComponent::getClassDef(), renderables);
			for (ZoneObject::ComponentList::iterator r = renderables.begin();
				r != renderables.end(); ++r)
			{
				RenderComponent* pRenderable = 
					static_cast<RenderComponent*>(*r);

				// update render component's affecting light list if needed. this
				// call will internally short-circuit if the object is not lit or
				// the light list doesn't need rebuilt
				pRenderable->updateLightList(pScene);
				pRenderable->queueForRendering(pRend);
			}
		}
	}

	pRend->endScene();
}
