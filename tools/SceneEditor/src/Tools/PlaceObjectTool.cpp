/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "PlaceObjectTool.h"
#include "CoreInterface.h"
#include "MessageBus.h"
#include "SceneData.h"
#include "View.h"
#include "UI/UserInterface.h"
#include "Base/Attribute.h"
#include "Game/InputEvent.h"
#include "Game/Scene.h"
#include "Game/Zone.h"
#include "Game/ZoneObject.h"
#include "Game/TerrainPatch.h"
#include "Game/Component_Render.h"
#include "Gfx/GfxCamera.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxUtil.h"
#include "Reflection/Reflection.h"
#include "Math/Vector2.h"
#include "Math/Ray.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
PlaceObjectTool::PlaceObjectTool()
{
	Attribute* pAttr = addAttribute(String("name")); 
	pAttr->setValue(String("Place"));
	pAttr = addAttribute(String("description")); 
	pAttr->setValue(String("Place Object Tool"));
	pAttr = addAttribute(String("icon"));
	pAttr->setValue(String("icons/place.ico"));
}
//---------------------------------------------------------------------------
PlaceObjectTool::~PlaceObjectTool()
{
}
//---------------------------------------------------------------------------
bool PlaceObjectTool::update(float /*deltaT*/, Data* /*pData*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool PlaceObjectTool::handleInputEvent(const InputEvent& event, Data* pData)
{
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
						if ((*it)->getClassId() == TerrainPatch::Class)
						{
							pSelected = *it;
							break;
						}
					}
				}

				// place currently-selected object at the intersection point
				Vector4 intersection;
				size_t ct = 1;
				pScene->getIntersectionPoints(ray, &intersection, ct);

				// get what it is we are supposed to place
				placeObject(
					CoreInterface::get().pUI->getSelectedTemplate(), 
					pScene,
					intersection);
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
void PlaceObjectTool::activate()
{
	// show the object template list in the UI
	CoreInterface::get().pUI->showTemplateList(true);
	Tool::activate();
}
//---------------------------------------------------------------------------
void PlaceObjectTool::deactivate()
{
	// hide the object template list in the UI
	CoreInterface::get().pUI->showTemplateList(false);
	Tool::deactivate();
}
//---------------------------------------------------------------------------
size_t PlaceObjectTool::getNumRenderables()
{
	return 0;
}
//---------------------------------------------------------------------------
bool PlaceObjectTool::getRenderable(GfxMeshInstance& inst, size_t index)
{
	inst;
	index;
	return false;
}
//---------------------------------------------------------------------------
bool PlaceObjectTool::findIntersection( 
	/*in*/const Ray& ray, 
	/*in*/const ZoneObject* pSelected, 
	/*out*/Vector4& intersection)
{
	if (!pSelected)
		return false;

	RenderComponent* pRend;
	if (!pSelected->findComponents(RenderComponent::getClassDef(), (Component**)&pRend))
		return false;

	GfxMesh* pMesh = AutoResPtr<GfxMesh>(pRend->getMeshInstance().getMeshHandle());
	if (!pMesh)
		return false;

	float t;
	Transform xform(const_cast<ZoneObject*>(pSelected)->getTransform());
	if (!GfxUtil::collideRayWithMesh(ray, xform, *pMesh, t))
		return false;

	intersection = ray.end + ray.dir * t;
	return true;
}
//---------------------------------------------------------------------------
void PlaceObjectTool::placeObject(
	Reflection::Object* pTemplate, 
	Scene* pScene,
	const Vector4& pos)
{
	if (!pScene || !pTemplate)
		return;

	if (!pTemplate->getDerivedClassDef()->isA(ZoneObject::getClassDef()))
		return;

	ZoneObject* pZoneObj = ObjectLibrary::instance()->clone(static_cast<ZoneObject*>(pTemplate));
	pZoneObj->setTransform(Transform(pos, Quaternion::IDENTITY, Vector4::UNIT_SCALE));
	pZoneObj->initialize();

	CoreInterface::get().pMsgBus->ZoneObjectCreated(pZoneObj);
	pScene->getCurrentZone()->addObject(pZoneObj);
	CoreInterface::get().pMsgBus->ZoneObjectAdded(pScene->getCurrentZone(), pZoneObj);
}
