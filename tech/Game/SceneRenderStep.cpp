/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "SceneRenderStep.h"
#include "Scene.h"
#include "Zone.h"
#include "ZoneObject.h"
#include "Component_Render.h"
#include "Gfx/Camera.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Reflection/Reflection.h"
#include "Reflection/ClassDef.h"

using namespace Teardrop;
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
    const VisibleObjects& objects, Gfx::Renderer* pRend, Scene* pScene)
{
	if (!m_pCamera)
		return;

	// then update/render the main scene
    //pRend->setRenderMode(Gfx::Renderer::RENDER_DEFAULT);
	m_pRT->setCurrent();
//	pRend->clearRenderTarget(); // clears all
//	pRend->setColorWrite(true);
	m_pCamera->setAspect(m_pRT->aspect());
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
