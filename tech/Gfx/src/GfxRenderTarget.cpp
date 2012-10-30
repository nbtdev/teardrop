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
#include "GfxRenderTarget.h"
#include "GfxRenderConfig.h"
#include "GfxViewport.h"
#include "Util/include/Hash.h"
#include <map>
#include <new.h>

using namespace CoS;
//---------------------------------------------------------------------------
const unsigned __int64 GfxRenderTarget::IID = hashString64("GfxRenderTarget");
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
	GfxViewport* pVP = COS_NEW GfxViewport;
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
bool GfxRenderTarget::queryInterface(unsigned __int64 IID, void** ppInterface)
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
