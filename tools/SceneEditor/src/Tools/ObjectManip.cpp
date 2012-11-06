/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ObjectManip.h"
#include "TransformGizmo.h"
#include "InputFilter.h"
#include "Gfx/GfxMeshInstance.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxUtil.h"
#include "Gfx/GfxCamera.h"
#include "Gfx/GfxRenderWindow.h"
#include "Gfx/GfxRenderTarget.h"
#include "Resource/ResourceHandle.h"
#include "Resource/ResourceManager.h"
#include "Game/Zone.h"
#include "Math/Ray.h"
#include "Math/Sphere.h"
#include "Math/Query.h"
#include "Memory/Memory.h"
#include <list>

using namespace Teardrop;
//---------------------------------------------------------------------------
struct ObjectManip::_Listeners 
	: public std::list<ObjectManip::Listener*> 
{
	TD_DECLARE_ALLOCATOR();
};
//---------------------------------------------------------------------------
static ::InputFilter s_inputFilter;
//---------------------------------------------------------------------------
ObjectManip::ObjectManip()
{
	m_pCamera = 0;
	m_pZone = 0;
	m_pGizmo = 0;
	m_pSelected = 0;
	m_bEnabled = false;
	m_bLMB = false;
	m_mode = MANIP_NONE;
	m_rotateStartAngle = MathUtil::NEGATIVE_INFINITY;
	m_rotateStart = Quaternion::IDENTITY;
	m_transScaleStart = Vector4::NEGATIVE_INFINITY;
	m_transOriginal = Vector4::ZERO;

	m_pListeners = TD_NEW _Listeners;
}
//---------------------------------------------------------------------------
ObjectManip::~ObjectManip()
{
	delete m_pListeners;
}
//---------------------------------------------------------------------------
struct Vert
{
	float x, y, z;
	unsigned char b, g, r, a;
};
//---------------------------------------------------------------------------
bool ObjectManip::initialize()
{
	// create gizmo
	m_pGizmo = TD_NEW TransformGizmo();
	m_pGizmo->initialize();
	//m_pGizmo->show();
	s_inputFilter.disable();

	return true;
}
//---------------------------------------------------------------------------
bool ObjectManip::destroy()
{
	// destroy gizmo
	m_pGizmo->destroy();
	delete m_pGizmo;
	m_pGizmo = 0;

	return true;
}
//---------------------------------------------------------------------------
bool ObjectManip::update(float deltaT)
{
	if (m_pGizmo && m_pSelected)
	{
		m_pGizmo->update(deltaT);
	}

	return true;
}
//---------------------------------------------------------------------------
static const GfxMeshInstance s_empty;
const GfxMeshInstance& ObjectManip::getRenderable(size_t index) const
{
	if (m_bEnabled && m_pGizmo && m_pGizmo->isVisible())
	{
		return m_pGizmo->getMeshInstance(index);
	}

	return s_empty;
}
//---------------------------------------------------------------------------
bool ObjectManip::isEnabled() const
{
	return m_bEnabled;
}
//---------------------------------------------------------------------------
void ObjectManip::setEnabled(bool enabled)
{
	m_bEnabled = enabled;
	enabled ? s_inputFilter.enable() : s_inputFilter.disable();
}
//---------------------------------------------------------------------------
ZoneObject* ObjectManip::getSelectedObject()
{
	return m_pSelected;
}
//---------------------------------------------------------------------------
void ObjectManip::setCamera(GfxCamera* pCam)
{
	m_pCamera = pCam;
}
//---------------------------------------------------------------------------
void ObjectManip::setZone(Zone* pZone)
{
	m_pZone = pZone;
}
//---------------------------------------------------------------------------
void ObjectManip::setRenderWindow(GfxRenderTarget* pWin)
{
	m_pWindow = pWin;
}
//---------------------------------------------------------------------------
void ObjectManip::addListener(Listener* pN)
{
	for (_Listeners::iterator it = m_pListeners->begin();
		it != m_pListeners->end(); ++it)
	{
		if (*it == pN)
			return;
	}

	m_pListeners->push_back(pN);
}
//---------------------------------------------------------------------------
void ObjectManip::removeListener(Listener* pN)
{
	for (_Listeners::iterator it = m_pListeners->begin();
		it != m_pListeners->end(); ++it)
	{
		if (*it == pN)
		{
			m_pListeners->erase(it);
			return;
		}
	}
}
//---------------------------------------------------------------------------
Teardrop::InputFilter& ObjectManip::getInputFilter()
{
	return s_inputFilter;
}
//---------------------------------------------------------------------------
ZoneObject* ObjectManip::castRay(int x, int y)
{
	if (m_pCamera && m_pWindow && !m_bLMB)
	{
		float nx = (float)x / (float)m_pWindow->getWidth();
		float ny = (float)y / (float)m_pWindow->getHeight();

		Ray ray;
		m_pCamera->getViewportWorldRay(ray, nx, ny);

		ZoneObjects objects;
		m_pZone->getIntersectingObjects(ray, objects, true);
		if (objects.size())
		{
			// filter out terrain patches (their AABB get in the way)
			for (ZoneObjects::iterator it = objects.begin();
				it != objects.end(); ++it)
			{
				if ((*it)->getClassId() != TerrainPatch::Class)
					return *it;
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
void ObjectManip::notifySelectionChanged()
{
	for (_Listeners::iterator it = m_pListeners->begin();
		it != m_pListeners->end(); ++it)
	{
		if (*it)
		{
			(*it)->OnObjectChanged(m_pSelected);
		}
	}
}
//---------------------------------------------------------------------------
void ObjectManip::setTarget(ZoneObject* pObj)
{
	m_pSelected = pObj;

	m_pGizmo->setObject(pObj);
	pObj == 0 ? m_pGizmo->hide() : m_pGizmo->show();
}
//---------------------------------------------------------------------------
void ObjectManip::pick(int x, int y)
{
	ZoneObject* pObj = castRay(x, y);

	if (pObj)
	{
		m_pGizmo->setObject(pObj);
		m_pGizmo->show();

		if (m_pSelected != pObj)
		{
			m_pSelected = pObj;
			notifySelectionChanged();
		}
	}
	else
	{
		m_pGizmo->hide();
	}
}
//---------------------------------------------------------------------------
bool ObjectManip::hasActiveOperation(int x, int y)
{
	scanForOperations(x, y);
	return (m_pGizmo->getOperations() != TransformGizmo::OP_NONE);
}
//---------------------------------------------------------------------------
void ObjectManip::scanForOperations(int x, int y)
{
	Ray ray;
	float nx = (float)x / (float)m_pWindow->getWidth();
	float ny = (float)y / (float)m_pWindow->getHeight();
	m_pCamera->getViewportWorldRay(ray, nx, ny);

	Vector4 pt;
	m_pGizmo->castRay(ray, pt);

	// reset any stored angular displacement and the starting quaternion
	m_rotateStartAngle = MathUtil::NEGATIVE_INFINITY;
	m_transScaleStart = Vector4::NEGATIVE_INFINITY;
	m_rotateStart = Quaternion::IDENTITY;
	m_transOriginal = Vector4::NEGATIVE_INFINITY;
}
//---------------------------------------------------------------------------
ObjectManip::ManipulationMode ObjectManip::getMode() const
{
	return m_mode;
}
//---------------------------------------------------------------------------
void ObjectManip::setMode(ManipulationMode mode)
{
	m_mode = mode;
	switch (mode)
	{
	case MANIP_TRANSLATE:
		m_pGizmo->setGizmo(TransformGizmo::GIZMO_POSITION);
		break;

	case MANIP_ROTATE:
		m_pGizmo->setGizmo(TransformGizmo::GIZMO_ROTATION);
		break;

	case MANIP_SCALE:
		m_pGizmo->setGizmo(TransformGizmo::GIZMO_SCALE);
		break;

	default:
		m_pGizmo->hide();
		break;
	}
}
//---------------------------------------------------------------------------
void ObjectManip::manipulate(int rx, int ry, int /*rz*/)
{
	manipObject(rx, ry);
}
//---------------------------------------------------------------------------
void ObjectManip::manipObject(int rx, int ry)
{
	if (!m_pSelected)
		return;

	switch (m_mode)
	{
	case MANIP_TRANSLATE:
		translate(rx, ry);
		break;

	case MANIP_ROTATE:
		rotate(rx, ry);
		break;

	case MANIP_SCALE:
		scale(rx, ry);
		break;
	}
}
//---------------------------------------------------------------------------
void ObjectManip::translate(int rx, int ry)
{
	Transform xform(m_pSelected->getTransform());

	// if we are here, then the mouse had to be over part of the translate
	// gizmo, so cast a ray on the gizmo to see where we intersect now
	Ray ray;
	float nx = (float)rx / (float)m_pWindow->getWidth();
	float ny = (float)ry / (float)m_pWindow->getHeight();
	m_pCamera->getViewportWorldRay(ray, nx, ny);

	if (m_transScaleStart == Vector4::NEGATIVE_INFINITY)
	{
		Vector4 pt;
		if (m_pGizmo->castRay(ray, pt))
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
	int ops = m_pGizmo->getOperations();

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
		m_pSelected->setTransform(xform);
	}
}
//---------------------------------------------------------------------------
void ObjectManip::rotate(int rx, int ry)
{
	Transform xform(m_pSelected->getTransform());

	// fire a ray into the viewport
	Ray ray;
	float nx = (float)rx / (float)m_pWindow->getWidth();
	float ny = (float)ry / (float)m_pWindow->getHeight();
	m_pCamera->getViewportWorldRay(ray, nx, ny);

	// make a sphere of the gizmo
	Sphere sphere(xform.trans, m_pGizmo->getRadius());

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
	int ops = m_pGizmo->getOperations();

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
		m_pSelected->setTransform(xform);
	}
}
//---------------------------------------------------------------------------
void ObjectManip::scale(int /*rx*/, int /*ry*/)
{
}
//---------------------------------------------------------------------------
void ObjectManip::getManipFactors(
	int dx,		// in: screen-space x delta
	int dy,		// in: screen-space y delta
	float& x,	// out: object-local x-axis change factor
	float& y,	// out: object-local y-axis change factor
	float& z)	// out: object-local z-axis change factor
{
	/*
		The idea here is to have the change in whatever it is we are changing,
		happen in an intuitive fashion. That means when the user moves the 
		mouse along a particular axis, the object should change something
		that "looks" like it's aligned with that axis. In general, this means
		we have to project the objects three local axes onto the camera 
		projection plane and make calculations there (basically dot products
		against whatever axis it is we are interested in). Since we don't know 
		in here what it is we are doing, we simply will get the factors for the
		movement along all three of the object's local axes and the caller can 
		do with those results what it will.
	*/

	// get the screen-space mouse movement vector
	Vector4 mouseVec((float)-dx, (float)-dy, 0, 0);
	float dist = mouseVec.normalize();

	x = y = z = 0;

	if (dist == 0)
		return;

	// transform the world-space local axes to screen-space projections
	Matrix44 rotMat;
	Matrix44 xf;
	Transform xform(m_pSelected->getTransform());
	xform.rot.toRotationMatrix(rotMat);

	Matrix44 viewProj;
	mul3(viewProj, m_pCamera->getViewMatrix(), m_pCamera->getProjectionMatrix());
	//mul3(xf, rotMat, viewProj);
	mul3(xf, rotMat, m_pCamera->getViewMatrix());

	Vector4 ssX, ssY, ssZ;
	mul3(ssX, xf, Vector4::UNIT_X);
	mul3(ssY, xf, Vector4::UNIT_Y);
	mul3(ssZ, xf, Vector4::UNIT_Z);

	x = dot(ssX, mouseVec) * dist;
	y = dot(ssY, mouseVec) * dist;
	z = dot(ssZ, mouseVec) * dist;
}
