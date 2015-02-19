/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxRenderTarget.h"
#include "GfxRenderConfig.h"
#include "GfxViewport.h"
#include "Util/Hash.h"
#include <map>
#include <cstdint>

using namespace Teardrop;
//---------------------------------------------------------------------------
const uint64_t GfxRenderTarget::IID = hashString64("GfxRenderTarget");
//---------------------------------------------------------------------------
GfxRenderTarget::GfxRenderTarget()
{
	m_fmt = FMT_R8G8B8;
	m_width = 0;
	m_height = 0;
	m_depth = 0;
	m_enabled = false;
	m_bResized = false;
}
//---------------------------------------------------------------------------
GfxRenderTarget::~GfxRenderTarget()
{
}
//---------------------------------------------------------------------------
bool GfxRenderTarget::initialize(
	GfxRenderConfig& cfg,
	SurfaceFormat fmt)
{
	m_width = cfg.width;
	m_height = cfg.height;
	m_depth = 1;
	m_fmt = fmt;
	m_enabled = true;
	m_bResized = false;
	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderTarget::destroy()
{
	removeAllViewports();
	return true;
}
//---------------------------------------------------------------------------
size_t GfxRenderTarget::getWidth() const
{
	return m_width;
}
//---------------------------------------------------------------------------
size_t GfxRenderTarget::getHeight() const
{
	return m_height;
}
//---------------------------------------------------------------------------
size_t GfxRenderTarget::getDepth() const
{
	return m_depth;
}
//---------------------------------------------------------------------------
SurfaceFormat GfxRenderTarget::getFormat() const
{
	return m_fmt;
}
//---------------------------------------------------------------------------
bool GfxRenderTarget::isEnabled() const
{
	return m_enabled;
}
//---------------------------------------------------------------------------
void GfxRenderTarget::setEnabled(bool enabled)
{
	m_enabled = enabled;
}
//---------------------------------------------------------------------------
GfxViewport* GfxRenderTarget::addViewport(
	float x, float y, float w, float h, size_t zorder)
{
	GfxViewport* pVP = TD_NEW GfxViewport;
	if (!pVP->initialize(this))
	{
		return 0;
	}

	pVP->setPosition(Vector2(x, y), true);
	pVP->setSize(Vector2(w, h), true);

	Viewports::value_type val(zorder, pVP);
	m_viewports.insert(val);
	return pVP;
}
//---------------------------------------------------------------------------
void GfxRenderTarget::removeViewport(GfxViewport* pVP)
{
	for (Viewports::iterator it = m_viewports.begin(); 
		it != m_viewports.end(); ++it)
	{
		if (it->second == pVP)
		{
			delete it->second;
			m_viewports.erase(it);
			break;
		}
	}
}
//---------------------------------------------------------------------------
const GfxRenderTarget::Viewports& GfxRenderTarget::getViewports() const
{
	return m_viewports;
}
//---------------------------------------------------------------------------
void GfxRenderTarget::removeAllViewports()
{
	if (!m_viewports.size())
		return;

	for (Viewports::iterator it = m_viewports.begin(); 
		it != m_viewports.end(); ++it)
	{
		delete it->second;
	}

	m_viewports.clear();
}
//---------------------------------------------------------------------------
bool GfxRenderTarget::handlePendingResize()
{
	return false;
}
//---------------------------------------------------------------------------
bool GfxRenderTarget::queryInterface(uint64_t IID, void** ppInterface)
{
	if (!ppInterface)
		return false;

	if (IID == GfxRenderTarget::IID)
	{
		*ppInterface = this;
		return true;
	}

	*ppInterface = 0;
	return false;
}
