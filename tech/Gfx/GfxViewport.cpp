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
#include "GfxViewport.h"
#include "GfxRenderTarget.h"
#include "GfxMeshInstance.h"

using namespace CoS;
//---------------------------------------------------------------------------
GfxViewport::GfxViewport()
{
	m_pos = Vector2::ZERO;
	m_size = Vector2::ZERO;
	m_posNorm = Vector2::ZERO;
	m_sizeNorm = Vector2::ZERO;
	m_pRT = 0;
	m_bEnabled = false;
	m_bClearEachRender = true;
	m_pVisibleMeshes = 0;
	m_numVisibleMeshes = 0;
}
//---------------------------------------------------------------------------
GfxViewport::~GfxViewport()
{
}
//---------------------------------------------------------------------------
bool GfxViewport::initialize(GfxRenderTarget* pRT)
{
	m_pRT = pRT;
	return true;
}
//---------------------------------------------------------------------------
bool GfxViewport::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
const Vector2& GfxViewport::getPosition(bool normalized) const
{
	if (normalized)
		return m_posNorm;
	else
		return m_pos;
}
//---------------------------------------------------------------------------
const Vector2& GfxViewport::getSize(bool normalized) const
{
	if (normalized)
		return m_sizeNorm;
	else
		return m_size;
}
//---------------------------------------------------------------------------
void GfxViewport::setPosition(const Vector2& pos, bool normalized)
{
	m_pos = pos;
	if (normalized)
	{
		m_posNorm = pos;
		m_pos.x *= (float)m_pRT->getWidth();
		m_pos.y *= (float)m_pRT->getHeight();
	}
	else
	{
		m_posNorm.x = m_pos.x / (float)m_pRT->getWidth();
		m_posNorm.y = m_pos.y / (float)m_pRT->getHeight();
	}
}
//---------------------------------------------------------------------------
void GfxViewport::translate(const Vector2& pos, bool normalized)
{
	Vector2 p(pos);

	if (normalized)
	{
		p.x *= (float)m_pRT->getWidth();
		p.y *= (float)m_pRT->getHeight();
	}

	setPosition(m_pos + p, false);
}
//---------------------------------------------------------------------------
void GfxViewport::setSize(const Vector2& size, bool normalized)
{
	m_size = size;
	if (normalized)
	{
		m_sizeNorm = size;
		m_size.x *= (float)m_pRT->getWidth();
		m_size.y *= (float)m_pRT->getHeight();
	}
	else
	{
		m_sizeNorm.x = m_size.x / (float)m_pRT->getWidth();
		m_sizeNorm.y = m_size.y / (float)m_pRT->getHeight();
	}
}
//---------------------------------------------------------------------------
const Matrix44& GfxViewport::getViewMatrix() const
{
	return m_view;
}
//---------------------------------------------------------------------------
const Matrix44& GfxViewport::getProjectionMatrix() const
{
	return m_proj;
}
//---------------------------------------------------------------------------
void GfxViewport::setViewMatrix(const Matrix44& view)
{
	m_view = view;
}
//---------------------------------------------------------------------------
void GfxViewport::setProjectionMatrix(const Matrix44& proj)
{
	m_proj = proj;
}
//---------------------------------------------------------------------------
void GfxViewport::setVisibleMeshes(const GfxMeshInstance* pMeshes, size_t ct)
{
	// if somehow we still have a pointer value, discard the existing block
	if (m_pVisibleMeshes)
	{
		GFX_FREE(m_pVisibleMeshes);
	}

	m_numVisibleMeshes = ct;
	m_pVisibleMeshes = 0;

	if (pMeshes && ct)
	{
		// copy the mesh instance block
		m_pVisibleMeshes = (GfxMeshInstance*)GFX_ALLOCATE(sizeof(GfxMeshInstance) * ct);
		memcpy(m_pVisibleMeshes, pMeshes, sizeof(GfxMeshInstance) * ct);
	}
}
//---------------------------------------------------------------------------
const GfxMeshInstance* GfxViewport::getVisibleMeshes() const
{
	return m_pVisibleMeshes;
}
//---------------------------------------------------------------------------
size_t GfxViewport::getNumVisibleMeshes() const
{
	return m_numVisibleMeshes;
}
//---------------------------------------------------------------------------
const GfxRenderTarget* GfxViewport::getRenderTarget() const
{
	return m_pRT;
}
//---------------------------------------------------------------------------
bool GfxViewport::isEnabled() const
{
	return m_bEnabled;
}
//---------------------------------------------------------------------------
void GfxViewport::enable(bool enabled)
{
	m_bEnabled = enabled;
}
//---------------------------------------------------------------------------
bool GfxViewport::getClearEachFrame() const
{
	return m_bClearEachRender;
}
//---------------------------------------------------------------------------
void GfxViewport::setClearEachFrame(bool clear)
{
	m_bClearEachRender = clear;
}
//---------------------------------------------------------------------------
void GfxViewport::updateDimensions()
{
	m_pos = Vector2(
		m_posNorm.x * (float)m_pRT->getWidth(),
		m_posNorm.y * (float)m_pRT->getHeight());

	m_size = Vector2(
		m_sizeNorm.x * (float)m_pRT->getWidth(),
		m_sizeNorm.y * (float)m_pRT->getHeight());
}
