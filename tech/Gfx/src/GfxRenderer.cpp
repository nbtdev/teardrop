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
#include "GfxRenderer.h"
#include "GfxVertexData.h"
#include "GfxIndexData.h"
#include "GfxTexture.h"
#include "GfxMesh.h"
#include "GfxMeshInstance.h"
#include "GfxResourceFactory.h"
#include "GfxRenderStats.h"
#include "GfxRenderWindow.h"
#include "GfxMatrixPaletteCache.h"
#include "Memory/include/Memory.h"
#include "Thread/include/ThreadUtil.h"
#include "Util/include/Logger.h"
#include "Util/include/Environment.h"
#include "Resource/include/ResourceManager.h"

using namespace CoS;
//---------------------------------------------------------------------------
GfxRenderer::GfxRenderer(Environment& env)
: m_env(env)
{
	m_frameNumber = 0;
	m_pResourceFactory = 0;
	m_pMainWindow = 0;
	m_createMainWindow = false;
	m_renderMode = RENDER_DEFAULT;
}
//---------------------------------------------------------------------------
GfxRenderer::~GfxRenderer()
{
}
//---------------------------------------------------------------------------
bool GfxRenderer::initialize(
	const GfxRenderConfig& cfg,
	bool createMainWindow)
{
	m_cfg = cfg;
	m_createMainWindow = createMainWindow;

	if (!m_env.isOffline)
	{
		m_cfg.pLogger->logMessage("Initializing gfx");
		m_cfg.pLogger->logMessage(m_cfg.bNVPerfHUDAdapter?"Using NVPerfHUD Adapter":"NOT Using NVPerfHUD Adapter");

		// make a new Gfx resource factory
		m_pResourceFactory = COS_NEW GfxResourceFactory(m_env);

		// register our factory for mesh and texture types
		m_env.pResourceMgr->registerFactory(
			m_pResourceFactory, GfxMesh::RESOURCE_TYPE);
		m_env.pResourceMgr->registerFactory(
			m_pResourceFactory, GfxTexture::RESOURCE_TYPE);
	}
	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderer::destroy()
{
	// unregister our factories
	m_env.pResourceMgr->unregisterFactory(
		m_pResourceFactory, GfxMesh::RESOURCE_TYPE);
	m_env.pResourceMgr->unregisterFactory(
		m_pResourceFactory, GfxTexture::RESOURCE_TYPE);

	m_env.pResourceMgr->destroy();
	
	delete m_pResourceFactory;

	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderer::update()
{
	// Windows housekeeping
	MSG msg;
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			return false;
		}

		DispatchMessage(&msg);
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderer::_swapImpl(GfxRenderStats& /*stats*/)
{
	// does nothing -- must be implemented by derived classes
	return false;
}
//---------------------------------------------------------------------------
bool GfxRenderer::swap(GfxRenderStats& stats)
{
	// advance the frame number
	++m_frameNumber;

	return update() && _swapImpl(stats);
}
//---------------------------------------------------------------------------
// todo: implement better way to do this
#include "src/Direct3D9/GfxRendererD3D9.h"
//---------------------------------------------------------------------------
GfxRenderer* GfxRenderer::allocate(Environment& env, Allocator* pAlloc) 
{
	// for now just do D3D9
	if (pAlloc)
	{
		setGfxAllocator(pAlloc);
	}

	return COS_NEW GfxRendererD3D9(env);
}
//---------------------------------------------------------------------------
void GfxRenderer::deallocate(GfxRenderer* pRend) 
{
	if (pRend)
	{
		delete pRend;
	}
}
//---------------------------------------------------------------------------
GfxRenderTarget* GfxRenderer::getMainWindow() const 
{
	return m_pMainWindow;
}
//---------------------------------------------------------------------------
GfxRenderTarget* GfxRenderer::createRenderTarget(
	GfxRenderConfig& /*cfg*/,
	CoS::SurfaceFormat /*fmt*/,
	RenderTargetType /*type*/)
{
	// needs to be implemented by derived classes
	return 0;
}
//---------------------------------------------------------------------------
void GfxRenderer::destroyRenderTarget(GfxRenderTarget* /*pRT*/)
{
	// needs to be implemented by derived classes
}
//---------------------------------------------------------------------------
void GfxRenderer::queueForRendering(const GfxMeshInstance& /*rInst*/)
{
	// unreferenced param warning -- the only reason these methods have a 
	// definition here is because offline tools need a renderer too

	// needs to be implemented by derived classes
}
//---------------------------------------------------------------------------
void GfxRenderer::setRenderMode(RenderMode mode)
{
	m_renderMode = mode;
}
