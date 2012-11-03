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
#include "RotateObjectTool.h"
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
#include "Math/Sphere.h"

using namespace CoS;
//---------------------------------------------------------------------------
RotateObjectTool::RotateObjectTool(TransformGizmo& gizmo)
: m_gizmo(gizmo)
{
	Attribute* pAttr = addAttribute(String("name")); 
	pAttr->setValue(String("Rotate"));
	pAttr = addAttribute(String("description")); 
	pAttr->setValue(String("Rotate Tool"));
	pAttr = addAttribute(String("icon"));
	pAttr->setValue(String("icons/rotate.ico"));

	m_bLMB = false;
	m_bRotating = false;

	m_rotateStartAngle = MathUtil::NEGATIVE_INFINITY;
	m_rotateStart = Quaternion::IDENTITY;
}
//---------------------------------------------------------------------------
RotateObjectTool::~RotateObjectTool()
{
}
//---------------------------------------------------------------------------
bool RotateObjectTool::update(float deltaT, Data* /*pData*/)
{
	if (!m_gizmo.isInitialized())
		m_gizmo.initialize();

	m_gizmo.update(deltaT);
	return true;
}
//---------------------------------------------------------------------------
bool RotateObjectTool::handleInputEvent(const InputEvent& event, Data* pData)
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
				m_bRotating = false;
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

		if (!m_bRotating)
		{
			// then, if we are not already doing something, check to see if the 
			// mouse got moved over the gizmo
			Vector4 pt;
			m_gizmo.castRay(ray, pt);

			// reset any stored angular displacement and the starting quaternion
			m_rotateStartAngle = MathUtil::NEGATIVE_INFINITY;
			m_rotateStart = Quaternion::IDENTITY;

			m_bRotating = m_bLMB && (m_gizmo.getOperations() != TransformGizmo::OP_NONE);
		}

		if (m_bRotating)
		{
			rotate(event.mouse.absX, event.mouse.absY);
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
void RotateObjectTool::activate()
{
	Tool::activate();
	m_gizmo.setGizmo(TransformGizmo::GIZMO_ROTATION);
	m_gizmo.show();
}
//---------------------------------------------------------------------------
void RotateObjectTool::deactivate()
{
	m_gizmo.hide();
	Tool::deactivate();
}
//---------------------------------------------------------------------------
size_t RotateObjectTool::getNumRenderables()
{
	// gizmo is a 2-pass affair
	return (m_gizmo.isVisible() && m_gizmo.getObject()) ? 2 : 0; 
}
//---------------------------------------------------------------------------
bool RotateObjectTool::getRenderable(GfxMeshInstance& inst, size_t index)
{
	if (index < 2)
	{
		inst = m_gizmo.getMeshInstance(index);
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void RotateObjectTool::_onViewChanged()
{
}
//---------------------------------------------------------------------------
void RotateObjectTool::selectedObjectChanged(
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
void RotateObjectTool::rotate(int rx, int ry)
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

	// make a sphere of the gizmo
	Sphere sphere(xform.trans, m_gizmo.getRadius());

	/* fire a ray to see if we hit the sphere. if the ray hits the sphere, then
		t and q will have valid values in them, and we can proceeed. If not, 
		then we need to figure out a point on the sphere from the ray so that
		q can have a valid value before proceeding.
	*/

	float t;
	Vector4 q;
	Vector4 e;
	if (!Query::Ray_Sphere(ray, sphere, t, q))
	{
		/*	ray missed sphere, so in order to figure out a point on the 
			sphere we can use to get the azimuth around the axis of rotation,
			we need to figure out the point on the ray nearest to the center
			of the sphere, and from that direction (and the sphere radius), we
			can get a point of intersetion on the sphere.
		*/
		Query::ClosestPoint_Ray_Point(ray, sphere.m_center, t, q);
	}

	e = q - sphere.m_center;
	e.normalize();

	// figure out which one we are adjusting
	int ops = m_gizmo.getOperations();

	// rot is the axis of rotation; origin is the axis against
	// which we compare the projection to find out the angle; axisRot
	// is also the normal vector to the plane of rotation
	Vector4 axisRot = Vector4::ZERO;
	Vector4 axisOrigin = Vector4::ZERO;
	Vector4 u(Vector4::ZERO);
	float* c = &u.w;

	if (ops & TransformGizmo::OP_ROTATE_X)
	{
		axisRot = Vector4::UNIT_X;
		axisOrigin = Vector4::UNIT_Z;
		c = &u.y;
	}
	else if (ops & TransformGizmo::OP_ROTATE_Y)
	{
		axisRot = Vector4::UNIT_Y;
		axisOrigin = Vector4::UNIT_X;
		c = &u.z;
	}
	else if (ops & TransformGizmo::OP_ROTATE_Z)
	{
		axisRot = Vector4::UNIT_Z;
		axisOrigin = Vector4::UNIT_Y;
		c = &u.x;
	}

	if (axisRot != Vector4::ZERO)
	{
		/*
			The orthogonal projection U of some vector V onto 
			the plane is given by the vector equation:

			U = V - (V dot N)N 
		*/

		u = e - axisRot * dot(e, axisRot);
		u.normalize();
		float angle = MathUtil::acos(dot(u, axisOrigin));

		// handle the reflection around 180 degrees
		if (*c < 0)
			angle = MathUtil::TWO_PI - angle;

		// if our stored values are uninitialized, initialize them
		if (m_rotateStartAngle == MathUtil::NEGATIVE_INFINITY)
		{
			m_rotateStartAngle = angle;
		}

		if (m_rotateStart == Quaternion::IDENTITY)
		{
			m_rotateStart = xform.rot;
		}

		// now we have an azimuth around the axis of rotation, and we can
		// make a quaternion of that, and adjust the object's orientation 
		Quaternion quat(m_rotateStartAngle - angle, axisRot);

		// set the starting orientation plus this rotation as the new rot
		xform.rot = m_rotateStart * quat;
		pObj->setTransform(xform);
	}
}
