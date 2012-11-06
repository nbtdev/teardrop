/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "DebugMeshRenderStep.h"
#include "Component_Physics.h"
#include "ZoneObject.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxViewport.h"
#include "Gfx/GfxRenderTarget.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
DebugMeshRenderStep::DebugMeshRenderStep()
{
	m_bEnabled = false;
}
//---------------------------------------------------------------------------
DebugMeshRenderStep::~DebugMeshRenderStep()
{
}
//---------------------------------------------------------------------------
void DebugMeshRenderStep::render(
	const VisibleObjects& objects, GfxRenderer* pRenderer, Scene* /*pScene*/)
{
	if (!m_bEnabled)
		return;

	// since we are rendering into a scene that is already set up with the proper
	// camera and viewport, we do not need beginScene() here; endScene() however
	// is what actually renders so we want that

	for (size_t i=0; i<objects.size(); ++i)
	{
		ZoneObject* pObject = objects[i];

		PhysicsComponent* pComp;
		if (!pObject->findComponents(PhysicsComponent::getClassDef(), (Component**)&pComp,1, true))
			continue;

		// allow dynamic updates to meshes
		pRenderer->queueForRendering(pComp->getDebugMesh());
	}

	pRenderer->endScene();
}
