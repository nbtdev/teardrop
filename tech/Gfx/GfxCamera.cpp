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
#include "GfxCamera.h"
#include "GfxRenderer.h"
#include "Math/MathUtil.h"
#include "Math/Vector4.h"
#include "Math/Ray.h"
#include "Util/Environment.h"
#include "Memory/Memory.h"
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
GfxCamera::GfxCamera()
{
	m_position = Vector4::UNIT_SCALE * 100;
	m_lookAt = Vector4::ZERO;
	m_upVector = Vector4(0, 1, 0, 0);
	m_orientation = Quaternion::IDENTITY;
	m_fovY = MathUtil::HALF_PI / 2.0f; // default to 45 degree half-field
	m_nearClip = 1.0f;
	m_farClip = 10000.0f;
	m_aspect = 4.0f / 3.0f;
	m_bNeedsUpdate = false;
	m_view = Matrix44::IDENTITY;
	m_proj = Matrix44::IDENTITY;
	m_bOrtho = false;
	m_width = 1;
	m_height = 1;
	ZeroMemory(m_frustum, sizeof(m_frustum));
}
//---------------------------------------------------------------------------
GfxCamera::~GfxCamera()
{
}
//---------------------------------------------------------------------------
bool GfxCamera::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxCamera::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxCamera::isOrtho() const
{
	return m_bOrtho;
}
//---------------------------------------------------------------------------
const Vector4& GfxCamera::getLookAt() const
{
	return m_lookAt;
}
//---------------------------------------------------------------------------
const Vector4& GfxCamera::getUpVector() const
{
	return m_upVector;
}
//---------------------------------------------------------------------------
float GfxCamera::getFovY() const
{
	return m_fovY;
}
//---------------------------------------------------------------------------
float GfxCamera::getNearClip() const
{
	return m_nearClip;
}
//---------------------------------------------------------------------------
float GfxCamera::getFarClip() const
{
	return m_farClip;
}
//---------------------------------------------------------------------------
float GfxCamera::getAspect() const
{
	return m_aspect;
}
//---------------------------------------------------------------------------
const Vector4& GfxCamera::getPosition() const
{
	return m_position;
}
//---------------------------------------------------------------------------
const Matrix44& GfxCamera::getViewMatrix() const
{
	return m_view;
}
//---------------------------------------------------------------------------
const Matrix44& GfxCamera::getProjectionMatrix() const
{
	return m_proj;
}
//---------------------------------------------------------------------------
const Matrix44& GfxCamera::getViewProjMatrix() const
{
	return m_viewproj;
}
//---------------------------------------------------------------------------
void GfxCamera::setLookAt(const Vector4& lookAt)
{
	m_lookAt = lookAt;

	// recalc orientation
	Vector4 dir(lookAt - m_position);
	dir.normalize();

	Vector4 right, up;
	cross(right, Vector4::UNIT_Y, dir);
	right.normalize();

	cross(up, dir, right);

	Matrix44 mat(Matrix44::IDENTITY);
	mat.m[0][0] = right.x;
	mat.m[0][1] = right.y;
	mat.m[0][2] = right.z;
	mat.m[1][0] = up.x;
	mat.m[1][1] = up.y;
	mat.m[1][2] = up.z;
	mat.m[2][0] = dir.x;
	mat.m[2][1] = dir.y;
	mat.m[2][2] = dir.z;

	m_orientation.fromRotationMatrix(mat);
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::setUpVector(const Vector4& upVector)
{
	m_upVector = upVector;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::setPosition(const Vector4& pos)
{
	m_position = pos;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::setOrtho(bool bOrtho, float width, float height)
{
	m_bOrtho = bOrtho;
	m_width = width;
	m_height = height;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::setAspect(float aspect)
{
	m_aspect = aspect;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::setFovY(float fovY)
{
	m_fovY = fovY;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::setNearClip(float nearClip)
{
	m_nearClip = nearClip;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::setFarClip(float farClip)
{
	m_farClip = farClip;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
bool GfxCamera::update()
{
	if (!m_bNeedsUpdate)
	{
		return true;
	}

	// update view and projection matrices
	Matrix44 tmp;
	//m_view.fromLookAtLH(m_position, m_lookAt, m_upVector);
	m_orientation.toRotationMatrix(tmp);
	tmp.m[3][0] = m_position.x;
	tmp.m[3][1] = m_position.y;
	tmp.m[3][2] = m_position.z;
	tmp.invert(m_view);

	if (m_bOrtho)
		m_proj.makeOrthoProjectionLH(m_width, m_height, m_nearClip, m_farClip);
	else
		m_proj.makePerspectiveProjectionLH(m_nearClip, m_farClip, m_fovY, m_aspect);

	// update frustum planes
	updateFrustum();

	// cache viewproj
	mul(m_viewproj, m_view, m_proj);
	m_bNeedsUpdate = false;

	return true;
}
//---------------------------------------------------------------------------
void GfxCamera::setOrientation(const Quaternion& ori)
{
	m_orientation = ori;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::rotate(float radians, const Vector4& axis)
{
	Quaternion q;
	q.fromAngleAxis(radians, axis);
	rotate(q);
}
//---------------------------------------------------------------------------
void GfxCamera::rotate(const Quaternion& q)
{
	m_orientation = m_orientation * q;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
void GfxCamera::yaw(float radians)
{
	rotate(radians, Vector4::UNIT_Y);
}
//---------------------------------------------------------------------------
void GfxCamera::pitch(float radians)
{
	rotate(radians, Vector4::UNIT_X);
}
//---------------------------------------------------------------------------
void GfxCamera::roll(float radians)
{
	rotate(radians, Vector4::UNIT_Z);
}
//---------------------------------------------------------------------------
const Quaternion& GfxCamera::getOrientation() const
{
	return m_orientation;
}
//---------------------------------------------------------------------------
void GfxCamera::translate(const Vector4& trans)
{
	m_position += m_orientation * trans;
	m_bNeedsUpdate = true;
}
//---------------------------------------------------------------------------
const Plane* GfxCamera::getFrustumPlanes() const
{
	return m_frustum;
}
//---------------------------------------------------------------------------
//Gribb/Hartmann method: 
//  http://www2.ravensoft.com/users/ggribb/plane%20extraction.pdf
//---------------------------------------------------------------------------
void GfxCamera::updateFrustum()
{
	Matrix44 viewProj;
	mul(viewProj, m_view, m_proj);

	// left
	m_frustum[0].n.x = viewProj.m[0][3] + viewProj.m[0][0];
	m_frustum[0].n.y = viewProj.m[1][3] + viewProj.m[1][0];
	m_frustum[0].n.z = viewProj.m[2][3] + viewProj.m[2][0];
	m_frustum[0].d   = viewProj.m[3][3] + viewProj.m[3][0];

	// right
	m_frustum[1].n.x = viewProj.m[0][3] - viewProj.m[0][0];
	m_frustum[1].n.y = viewProj.m[1][3] - viewProj.m[1][0];
	m_frustum[1].n.z = viewProj.m[2][3] - viewProj.m[2][0];
	m_frustum[1].d   = viewProj.m[3][3] - viewProj.m[3][0];

	// top
	m_frustum[2].n.x = viewProj.m[0][3] - viewProj.m[0][1];
	m_frustum[2].n.y = viewProj.m[1][3] - viewProj.m[1][1];
	m_frustum[2].n.z = viewProj.m[2][3] - viewProj.m[2][1];
	m_frustum[2].d   = viewProj.m[3][3] - viewProj.m[3][1];

	// bottom
	m_frustum[3].n.x = viewProj.m[0][3] + viewProj.m[0][1];
	m_frustum[3].n.y = viewProj.m[1][3] + viewProj.m[1][1];
	m_frustum[3].n.z = viewProj.m[2][3] + viewProj.m[2][1];
	m_frustum[3].d   = viewProj.m[3][3] + viewProj.m[3][1];

	// near
	m_frustum[4].n.x = viewProj.m[0][2];
	m_frustum[4].n.y = viewProj.m[1][2];
	m_frustum[4].n.z = viewProj.m[2][2];
	m_frustum[4].d   = viewProj.m[3][2];

	// far
	m_frustum[5].n.x = viewProj.m[0][3] - viewProj.m[0][2];
	m_frustum[5].n.y = viewProj.m[1][3] - viewProj.m[1][2];
	m_frustum[5].n.z = viewProj.m[2][3] - viewProj.m[2][2];
	m_frustum[5].d   = viewProj.m[3][3] - viewProj.m[3][2];

	for (int i=0; i<6; ++i)
	{
		float l = m_frustum[i].n.normalize();
		m_frustum[i].d /= l;
	}
}
//---------------------------------------------------------------------------
// borrowed liberally from the algorithm used in the DX SDK "Pick" sample
bool GfxCamera::getViewportWorldRay(/*out*/Ray& ray, float x, float y) const
{
	ray.end.w = ray.dir.w = 0;

	Matrix44 inv;
	if(!D3DXMatrixInverse((D3DXMATRIX*)&inv, 0, (D3DXMATRIX*)&m_view))
	//if (!m_view.invert(inv))
	{
		return false;
	}

	Vector4 v(
		 ((2.f * x) - 1.0f) / m_proj.m[0][0],
		-((2.f * y) - 1.0f) / m_proj.m[1][1],
		1,
		0);

	ray.dir.x = v.x * inv.m[0][0] + v.y * inv.m[1][0] + v.z * inv.m[2][0];
	ray.dir.y = v.x * inv.m[0][1] + v.y * inv.m[1][1] + v.z * inv.m[2][1];
	ray.dir.z = v.x * inv.m[0][2] + v.y * inv.m[1][2] + v.z * inv.m[2][2];
	ray.dir.normalize();

	ray.end.x = inv.m[3][0];
	ray.end.y = inv.m[3][1];
	ray.end.z = inv.m[3][2];

	return true;
}