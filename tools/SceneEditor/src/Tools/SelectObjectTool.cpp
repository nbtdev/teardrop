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

#include "stdafx.h"
#include "CoreInterface.h"
#include "MessageBus.h"
#include "SelectObjectTool.h"
#include "SceneData.h"
#include "View.h"
#include "Base/Attribute.h"
#include "Game/include/InputEvent.h"
#include "Game/include/Scene.h"
#include "Game/include/Zone.h"
#include "Game/include/ZoneObject.h"
#include "Game/include/TerrainPatch.h"
#include "Game/include/Component_Render.h"
#include "Gfx/include/GfxCamera.h"
#include "Gfx/include/GfxMesh.h"
#include "Gfx/include/GfxSubMesh.h"
#include "Gfx/include/GfxMaterial.h"
#include "Reflection/include/Reflection.h"
#include "Math/include/Vector2.h"
#include "Math/include/Ray.h"

using namespace CoS;
//---------------------------------------------------------------------------
SelectObjectTool::SelectObjectTool()
{
	Attribute* pAttr = addAttribute(String("name")); 
	pAttr->setValue(String("Select"));
	pAttr = addAttribute(String("description")); 
	pAttr->setValue(String("Select Tool"));
	pAttr = addAttribute(String("icon"));
	pAttr->setValue(String("icons/select.ico"));
}
//---------------------------------------------------------------------------
SelectObjectTool::~SelectObjectTool()
{
	for (SelectedInstances::iterator it = m_selectedInst.begin();
		it != m_selectedInst.end(); ++it)
	{
		size_t numMat = it->getMaterialCount();
		for (size_t i=0; i<numMat; ++i)
		{
			GfxMaterial* pMat = it->getMaterialByIndex(i);
			pMat->destroy();
			delete pMat;
		}

		it->destroy();
	}
}
//---------------------------------------------------------------------------
bool SelectObjectTool::update(float /*deltaT*/, Data* /*pData*/)
{
	// ensure that the selection highlight matches the object's transform, 
	// since it may have changed elsewhere without us knowing
	size_t i = 0;
	for (SelectedObjects::iterator it = m_selectedObjects.begin();
		it != m_selectedObjects.end(); ++it, ++i)
	{
		Reflection::Object* pObj = *it;
		if (pObj->getDerivedClassDef()->isA(ZoneObject::getClassDef()))
		{
			ZoneObject* pZoneObj = static_cast<ZoneObject*>(pObj);
			m_selectedInst[i].setTransform(pZoneObj->getTransform());
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool SelectObjectTool::handleInputEvent(const InputEvent& event, Data* pData)
{
	CoreInterface& iface = CoreInterface::get();

	if (!m_pView)
		return false;

	if (!pData)
		return false;

	const GfxCamera* pCam = m_pView->getCamera();
	if (!pCam)
		return false;

	Scene* pScene = static_cast<SceneData*>(pData)->getScene();
	if (!pScene)
		return false;

	Vector2 viewSize;
	m_pView->getViewSize(viewSize);

	if (event.evtType.type == InputEvent::MOUSE)
	{
		if (!m_pView->isInside(event.mouse.absX, event.mouse.absY))
			return false;

		bool bButtonEvent = 
			(event.mouse.btnsDown || event.mouse.btnsReleased);

		if (bButtonEvent && event.mouse.btnsDown == InputEvent::BUTTON_LEFT)
		{
			Ray ray;
			float x = float(event.mouse.absX) / viewSize.x;
			float y = float(event.mouse.absY) / viewSize.y;
			if (pCam->getViewportWorldRay(ray, x, y))
			{
				// find something under the ray
				// TODO: handle multiple selections
				ZoneObjects objects;
				ZoneObject* pSelected = 0;
				if (pScene->getCurrentZone()->getIntersectingObjects(
					ray, objects, true))
				{

					// filter out terrain patches (their AABB get in the way)
					for (ZoneObjects::iterator it = objects.begin();
						it != objects.end(); ++it)
					{
						if ((*it)->getClassId() != TerrainPatch::Class)
						{
							pSelected = *it;
							break;
						}
					}
				}

				// call out the object selection change
				iface.pMsgBus->ObjectSelectionChanged(this, pSelected);
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
void SelectObjectTool::activate()
{
	Tool::activate();
}
//---------------------------------------------------------------------------
void SelectObjectTool::deactivate()
{
	Tool::deactivate();
}
//---------------------------------------------------------------------------
size_t SelectObjectTool::getNumRenderables()
{
	return m_selectedInst.size();
}
//---------------------------------------------------------------------------
bool SelectObjectTool::getRenderable(GfxMeshInstance& inst, size_t index)
{
	if (index < m_selectedInst.size())
	{
		inst = m_selectedInst[index];
		return true;
	}
	
	return false;
}
//---------------------------------------------------------------------------
void SelectObjectTool::_onViewChanged()
{
}
//---------------------------------------------------------------------------
void SelectObjectTool::selectedObjectChanged(
	void* sender, Reflection::Object* pObj)
{
	Tool::selectedObjectChanged(sender, pObj);

	// clear out any current instances
	for (SelectedInstances::iterator it = m_selectedInst.begin();
		it != m_selectedInst.end(); ++it)
	{
		size_t numMat = it->getMaterialCount();
		for (size_t i=0; i<numMat; ++i)
		{
			GfxMaterial* pMat = it->getMaterialByIndex(i);
			pMat->destroy();
			delete pMat;
		}

		it->createMaterialOverrides(0);
	}
	m_selectedInst.clear();
	m_selectedInst.resize(m_selectedObjects.size());

	// make a new list based on the selected object(s)
	size_t i=0;
	for (SelectedObjects::iterator it = m_selectedObjects.begin(); 
		it != m_selectedObjects.end(); ++it)
	{
		if ((*it)->getDerivedClassDef()->isA(ZoneObject::getClassDef()))
		{
			ZoneObject* pObj = static_cast<ZoneObject*>(*it);

			RenderComponent* pRend;
			if (pObj->findComponents(RenderComponent::getClassDef(), (Component**)&pRend))
			{
				// copy the component's mesh instance
				m_selectedInst[i] = pRend->getMeshInstance();

				// create material overrides
				GfxMesh* pMesh = AutoResPtr<GfxMesh>(m_selectedInst[i].getMeshHandle());
				if (pMesh)
				{
					size_t numSM = pMesh->getNumSubMeshes();
					m_selectedInst[i].createMaterialOverrides(numSM);

					for (size_t sm = 0; sm < numSM; ++sm)
					{
						GfxMaterial* pM = COS_NEW GfxMaterial;
						*pM = *(pMesh->getSubMesh(sm)->getMaterial());
						pM->setFillMode(GfxMaterial::FILLMODE_WIREFRAME);
						pM->setDiffuse(0xFFFFFFFF);

						for (size_t s=0; s<pM->getNumTextureStages(); ++s)
							pM->setTextureStage(s, false);

						pM->setDepthBias(1);
						pM->setNumLights(0);
						m_selectedInst[i].setMaterialByIndex(sm, pM);
						m_selectedInst[i].m_bCastShadows = false;
						m_selectedInst[i].m_bReceiveShadows = false;
					}
				}
			}
		}

		++i;
	}
}
