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
#include "MoveObjectTool.h"
#include "CoreInterface.h"
#include "MessageBus.h"
#include "View.h"
#include "SceneData.h"
#include "Tools/TransformGizmo.h"
#include "Base/Attribute.h"
#include "Game/InputEvent.h"
#include "Game/ZoneObject.h"
#include "Game/Scene.h"
#include "Gfx/GfxCamera.h"
#include "Gfx/GfxViewport.h"
#include "Gfx/GfxRenderWindow.h"
#include "Math/MathUtil.h"
#include "Math/Vector2.h"
#include "Math/Ray.h"
#include "Math/Query.h"

using namespace CoS;
//---------------------------------------------------------------------------
MoveObjectTool::MoveObjectTool(TransformGizmo& gizmo)
: m_gizmo(gizmo)
{
	Attribute* pAttr = addAttribute(String("name")); 
	pAttr->setValue(String("Move"));
	pAttr = addAttribute(String("description")); 
	pAttr->setValue(String("Move Tool"));
	pAttr = addAttribute(String("icon"));
	pAttr->setValue(String("icons/move.ico"));

	m_bLMB = false;
	m_bMoving = false;

	m_transScaleStart = Vector4::NEGATIVE_INFINITY;
	m_transOriginal = Vector4::ZERO;
}
//---------------------------------------------------------------------------
MoveObjectTool::~MoveObjectTool()
{
}
//---------------------------------------------------------------------------
bool MoveObjectTool::update(float deltaT, Data* /*pData*/)
{
	if (!m_gizmo.isInitialized())
		m_gizmo.initialize();

	m_gizmo.update(deltaT);
	return true;
}
//---------------------------------------------------------------------------
bool MoveObjectTool::handleInputEvent(const InputEvent& event, Data* pData)
{
	CoreInterface& iface = CoreInterface::get();

	if (!m_pView)
		return false;

	if (event.evtType.type == InputEvent::MOUSE)
	{
		if (event.mouse.btnsDown || event.mouse.btnsReleased)
		{
			// we don't handle button presses outside the window
			if (!m_pView->isInside(event.mouse.absX, event.mouse.absY))
				return false;

			if (event.mouse.btnsDown & InputEvent::BUTTON_LEFT)
				m_bLMB = true;
			if (event.mouse.btnsReleased & InputEvent::BUTTON_LEFT)
			{
				m_bLMB = false;
				m_bMoving = false;
			}
		}

		Ray ray;
		const GfxCamera* pCam = m_pView->getCamera();
		Vector2 size;
		m_pView->getViewSize(size);

		float x(event.mouse.absX);
		float y(event.mouse.absY);
		float nx = x / size.x;
		float ny = y / size.y;
		pCam->getViewportWorldRay(ray, nx, ny);

		if (!m_bMoving)
		{
			// then, if we are not already doing something, check to see if the 
			// mouse got moved over the gizmo
			Vector4 pt;
			m_gizmo.castRay(ray, pt);

			// reset any stored angular displacement and the starting quaternion
			m_transScaleStart = Vector4::NEGATIVE_INFINITY;
			m_transOriginal = Vector4::NEGATIVE_INFINITY;

			m_bMoving = m_bLMB && (m_gizmo.getOperations() != TransformGizmo::OP_NONE);
		}

		if (m_bMoving)
		{
			translate(event.mouse.absX, event.mouse.absY);
			return true;
		}
		else
		{
			if (!pData)
				return false;

			SceneData* pSceneData = static_cast<SceneData*>(pData);
			Scene* pScene = pSceneData->getScene();
			
			if (pScene && m_bLMB)
			{
				// if we weren't moving and the user clicked the mouse,
				// then it's possible we selected something else (or nothing)
				ZoneObjects objects;
				ZoneObject* pObj = 0;
				pScene->getIntersectingObjects(ray, objects, true);
				if (objects.size())
				{
					// filter out terrain patches (their AABB get in the way)
					for (ZoneObjects::iterator it = objects.begin();
						it != objects.end(); ++it)
					{
						if ((*it)->getClassId() != TerrainPatch::Class)
						{
							pObj = *it;
							break;
						}
					}

					Reflection::Object* pSelected = 0;
					if (m_selectedObjects.size())
						pSelected = *(m_selectedObjects.begin());
					
					if (pSelected != pObj)
					{
						// if it's not what we already had, alert the world
						// (including ourselves via callback)
						iface.pMsgBus->ObjectSelectionChanged(this, pObj);
						return true;
					}
				}
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
void MoveObjectTool::activate()
{
	Tool::activate();
	m_gizmo.setGizmo(TransformGizmo::GIZMO_POSITION);
	m_gizmo.show();
}
//---------------------------------------------------------------------------
void MoveObjectTool::deactivate()
{
	m_gizmo.hide();
	Tool::deactivate();
}
//---------------------------------------------------------------------------
size_t MoveObjectTool::getNumRenderables()
{
	// gizmo is a 2-pass affair
	return (m_gizmo.isVisible() && m_gizmo.getObject()) ? 2 : 0; 
}
//---------------------------------------------------------------------------
bool MoveObjectTool::getRenderable(GfxMeshInstance& inst, size_t index)
{
	if (index < 2)
	{
		inst = m_gizmo.getMeshInstance(index);
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void MoveObjectTool::_onViewChanged()
{
}
//---------------------------------------------------------------------------
void MoveObjectTool::selectedObjectChanged(
	void* sender, Reflection::Object* pObj)
{
	if (!pObj ||
		(pObj && pObj->getDerivedClassDef()->isA(ZoneObject::getClassDef())))
	{
		// let the base do its thing
		Tool::selectedObjectChanged(sender, pObj);

		// set the object on the gizmo
		ZoneObject* pZObj = static_cast<ZoneObject*>(pObj);
		m_gizmo.setObject(pZObj);
	}
}
//---------------------------------------------------------------------------
void MoveObjectTool::translate(int rx, int ry)
{
	if (!m_selectedObjects.size())
		return;

	ZoneObject* pObj = static_cast<ZoneObject*>(*m_selectedObjects.begin());
	Transform xform(pObj->getTransform());

	// if we are here, then the mouse had to be over part of the translate
	// gizmo, so cast a ray on the gizmo to see where we intersect now
	Ray ray;
	const GfxCamera* pCam = m_pView->getCamera();
	Vector2 size;
	m_pView->getViewSize(size);

	float x(rx);
	float y(ry);
	float nx = x / size.x;
	float ny = y / size.y;
	pCam->getViewportWorldRay(ray, nx, ny);

	if (m_transScaleStart == Vector4::NEGATIVE_INFINITY)
	{
		Vector4 pt;
		if (m_gizmo.castRay(ray, pt))
		{
			// this should always be true because we are here in response to
			// the user clicking on the gizmo, and we don't come back in here
			// until the user releases the mouse button
			m_transOriginal = xform.trans;
		}
	}

	/*
		The way this one works is by figuring out the closest points on
		two (non-parallel) lines -- the first line is the ray we shoot into
		the scene, and the second is the line defined by the axis along which
		we are moving the object. The point on the axis line will be
		a displacement along that line, from the point at which we started
		moving the object (which we stored in m_transScaleStart). 
	*/
	Vector4 axis = Vector4::ZERO;
	int ops = m_gizmo.getOperations();

	if (ops & TransformGizmo::OP_TRANSLATE_X)
	{
		axis = Vector4::UNIT_X;
	}
	else if (ops & TransformGizmo::OP_TRANSLATE_Y)
	{
		axis = Vector4::UNIT_Y;
	}
	else if (ops & TransformGizmo::OP_TRANSLATE_Z)
	{
		axis = Vector4::UNIT_Z;
	}

	if (axis != Vector4::ZERO)
	{
		Vector4 p1, p2;
		float s, t;
		Ray obj(m_transOriginal, xform.rot * axis);

		Query::ClosestPoints_Ray_Ray(ray, obj, s, t, p1, p2);

		if (m_transScaleStart == Vector4::NEGATIVE_INFINITY)
		{
			m_transScaleStart = p2;
		}

		Vector4 disp(p2 - m_transScaleStart);
		xform.trans = m_transOriginal + disp;
		pObj->setTransform(xform);
	}
}
