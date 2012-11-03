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
#include "RenderWindow.h"
#include "MessageBus.h"
#include "SceneData.h"
#include "ToolRenderStep.h"
#include "Tools/Tool.h"
#include "Camera/CameraControllerManager.h"
#include "Game/Scene.h"
#include "Game/Zone.h"
#include "Game/ZoneObject.h"
#include "Game/SpawnPoint.h"
#include "Game/DirectionalLight.h"
#include "Game/Component_Render.h"
#include "Game/SceneRenderer.h"
#include "Game/ShadowRenderStep.h"
#include "Game/SceneRenderStep.h"
#include "Gfx/GfxCommon.h"
#include "Gfx/GfxRenderConfig.h"
#include "Gfx/GfxRenderTarget.h"
#include "Gfx/GfxRenderTexture.h"
#include "Gfx/GfxRenderWindow.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxCamera.h"
#include "Gfx/GfxViewport.h"
#include "Gfx/GfxMeshInstance.h"
#include "Gfx/GfxUtil.h"
#include "Math/Ray.h"
#include "Math/Plane.h"

using namespace CoS;
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(RenderWindow, wxPanel)
	EVT_SIZE(RenderWindow::OnSize)
END_EVENT_TABLE()
//---------------------------------------------------------------------------
RenderWindow::RenderWindow(wxWindow* pParent)
: wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	m_pWin = 0;
	m_pVP = 0;
	m_pCam = 0;
	m_pCamMgr = 0;
	m_pSceneRenderer = 0;
	m_pToolRenderStep = 0;
}
//---------------------------------------------------------------------------
RenderWindow::~RenderWindow()
{
	if (m_pWin && m_pVP)
		m_pWin->removeViewport(m_pVP);

	delete m_pCam;
	delete m_pCamMgr;
	delete m_pSceneRenderer;

	// we don't delete m_pToolRenderStep here because the scene renderer owns it
}
//---------------------------------------------------------------------------
void RenderWindow::update(float deltaT, Data* pData)
{
	m_pCamMgr->update(deltaT);
	if (!pData)
		return;

	GfxRenderer* pRend = Environment::get().pRenderer;
	SceneData* pSceneData = static_cast<SceneData*>(pData);
	Scene* pScene = pSceneData->getScene();
	if (!pScene)
		return;

	ZoneObjects objects;
	pScene->getCurrentZone()->getVisibleObjects(
		m_pCam->getFrustumPlanes(), objects);

	m_pToolRenderStep->setActiveTool(m_pActiveTool);

	// render the scene
	m_pSceneRenderer->render(objects, pRend, pScene, m_pCam);
}
//---------------------------------------------------------------------------
void RenderWindow::OnSize(wxSizeEvent& event)
{
	wxSize size(event.GetSize());
	float width = static_cast<float>(size.GetWidth());
	float height = static_cast<float>(size.GetHeight());

	// if this is the first resize of this window, the GfxRenderWindow will 
	// be null and needs to be created
	if (!m_pWin)
		createWindow(size.GetWidth(), size.GetHeight());

	float aspect = width / height;
	if (m_pCam)
		m_pCam->setAspect(aspect);

	// temporary?
	Environment::get().pInput->setExtents(size.GetWidth(), size.GetHeight());

	GfxRenderWindow* pWin;
	m_pWin->queryInterface(GfxRenderWindow::IID, (void**)&pWin);
	pWin->resizeWindow(size.GetWidth(), size.GetHeight());
}
//---------------------------------------------------------------------------
void RenderWindow::createWindow(int w, int h)
{
	assert(!m_pWin);

	GfxRenderer* pRend = Environment::get().pRenderer;

	GfxRenderConfig cfg;
	cfg.bNVPerfHUDAdapter = true;
	cfg.fullscreen = false;
	cfg.hWnd = (HWND)GetHWND();
	cfg.width = w;
	cfg.height = h;
	cfg.vsync = false;
	m_pWin = pRend->createRenderTarget(cfg, FMT_R8G8B8, GfxRenderer::RT_WINDOW);
	m_pVP = m_pWin->addViewport(0, 0, 1, 1);
	m_pCam = COS_NEW GfxCamera;
	m_pCam->setFarClip(5000);
	m_pCam->setNearClip(0.5f);
	m_pCam->setFovY(MathUtil::HALF_PI/2);
	m_pCam->setPosition(Vector4::UNIT_SCALE * 100);
	m_pCam->setLookAt(Vector4::ZERO);

	m_pCamMgr = COS_NEW CameraControllerManager(m_pCam);
	m_pCamMgr->changeController(CameraControllerManager::CCM_ORBIT);

	m_pSceneRenderer = COS_NEW SceneRenderer;
	m_pSceneRenderer->addStep(COS_NEW ShadowRenderStep);
	
	SceneRenderStep* pSceneStep = COS_NEW SceneRenderStep;
	pSceneStep->setCamera(m_pCam);
	pSceneStep->setViewport(m_pVP);
	pSceneStep->setRenderTarget(m_pWin);
	m_pSceneRenderer->addStep(pSceneStep);

	m_pToolRenderStep = COS_NEW ToolRenderStep;
	m_pSceneRenderer->addStep(m_pToolRenderStep);
}
//---------------------------------------------------------------------------
bool RenderWindow::handleInput(const InputEvent& evt)
{

	//if (evt.evtType.type == InputEvent::KEYBOARD)
	//{
	//	if (evt.key.keyCode == DIK_LALT || evt.key.keyCode == DIK_RALT)
	//	{
	//		CameraControllerManager::Controller c;
	//		if (evt.key.bKeyDown)
	//			c = CameraControllerManager::CCM_ORBIT;
	//		else
	//			c = CameraControllerManager::CCM_FREECAM;

	//		if (m_pCamMgr)
	//		{
	//			m_pCamMgr->changeController(c);
	//			return true;
	//		}
	//	}
	//}

	if (m_pCamMgr)
		return m_pCamMgr->handleInputEvent(evt);

	return false;
}
//---------------------------------------------------------------------------
const GfxCamera* RenderWindow::getCamera()
{
	return m_pCam;
}
//---------------------------------------------------------------------------
void RenderWindow::getViewSize(Vector2& size)
{
	wxSize sz = GetSize();
	size.x = float(sz.GetWidth());
	size.y = float(sz.GetHeight());
}
//---------------------------------------------------------------------------
bool RenderWindow::isInside(int x, int y)
{
	wxSize sz = GetSize();
	wxPoint loc = GetPosition();

	return (
		x >= loc.x && x < (loc.x + sz.GetWidth()) &&
		y >= loc.y && y < (loc.y + sz.GetHeight())
		);
}
//---------------------------------------------------------------------------
void RenderWindow::activate(MessageBus* pMsgBus)
{
	pMsgBus->ObjectSelectionChanged.connect(
		this, &RenderWindow::onSelectedObjectChanged);
}
//---------------------------------------------------------------------------
void RenderWindow::deactivate(MessageBus* pMsgBus)
{
	pMsgBus->ObjectSelectionChanged.disconnect(
		this, &RenderWindow::onSelectedObjectChanged);
}
//---------------------------------------------------------------------------
void RenderWindow::onSelectedObjectChanged(
	void* /*pSender*/, Reflection::Object* pObj)
{
	ZoneObject* pZObj = 0;

	if (pObj && pObj->getDerivedClassDef()->isA(ZoneObject::getClassDef()))
		pZObj = static_cast<ZoneObject*>(pObj);

	if (m_pCamMgr)
		m_pCamMgr->changeTarget(pZObj);
}
