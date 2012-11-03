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

#include "DebugMeshRenderStep.h"
#include "Component_Physics.h"
#include "ZoneObject.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxViewport.h"
#include "Gfx/GfxRenderTarget.h"

using namespace CoS;
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
