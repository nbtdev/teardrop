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
#include "SceneEditorApp.h"
#include "SceneEditorFrame.h"
#include "Controller.h"
#include "UI/UserInterface.h"
#include "SceneData.h"
#include "EntityData.h"
#include "Util/include/_String.h"

#if defined(COS_OPTION_MEMPROFILE)
#include "Memory/include/AllocationTracker.h"
#endif

using namespace CoS;
//---------------------------------------------------------------------------
COS_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
COS_ALLOCATOR_IMPL(DEFAULT, DefaultAllocator, DEFAULT, size_t(-1), size_t(-1))
COS_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, size_t(-1), size_t(-1))
//---------------------------------------------------------------------------
IMPLEMENT_APP(SceneEditorApp)
BEGIN_EVENT_TABLE(SceneEditorApp, wxApp)
	EVT_IDLE(SceneEditorApp::OnIdle)
	EVT_END_SESSION(SceneEditorApp::OnEndSession)
	//EVT_ACTIVATE_APP(SceneEditorApp::onAppActivate)
END_EVENT_TABLE()
//---------------------------------------------------------------------------
SceneEditorApp::SceneEditorApp()
: m_logger(m_logStrm)
{
	m_pFrame = 0;
	m_pController = 0;
	m_pData = 0;

	// this should serve to send idle only to the app, since no window should
	// in theory be asking for this event (update() is sent as part of the 
	// controller framework)
	wxIdleEvent::SetMode(wxIDLE_PROCESS_SPECIFIED); 
}
//---------------------------------------------------------------------------
SceneEditorApp::~SceneEditorApp()
{
}
//---------------------------------------------------------------------------
//void SceneEditorApp::onAppActivate(wxActivateEvent& evt)
//{
//	if (m_pController)
//		m_pController->activate(evt.GetActive());
//}
//---------------------------------------------------------------------------
void SceneEditorApp::OnIdle(wxIdleEvent& evt)
{
	Timer* pTimer = Environment::get().pMasterClock;
	pTimer->update();

	float deltaT = pTimer->getElapsedTime();

	// signal frame updates to whoever wants them
	m_pController->update(deltaT, m_pData);

	if (m_pData)
		m_pData->update(deltaT);

	Environment::get().pResourceMgr->update();
	evt.RequestMore(true);

	// don't hog the CPU, you're just a tool
	Sleep(5);
}
//---------------------------------------------------------------------------
bool SceneEditorApp::OnInit()
{
	Physics::setAllocator(GetDEFAULTAllocator());
	Animation::setAllocator(GetDEFAULTAllocator());
	String::setAllocator(GetDEFAULTAllocator());

	// add handlers for all image types
	wxImage::AddHandler(new wxICOHandler);

	// init the editor context
	if (!m_logStrm.open("SceneEditor.log", WRITE|TRUNCATE|TEXT))
	{
		// todo: let the user know
		return false;
	}

	m_timer.reset();

	Environment& env = Environment::get();
	env.pDataPath = ".\\data/";
	env.isOffline = false;
	env.pLogger = &m_logger;
	env.pRenderer = m_pRend = GfxRenderer::allocate(env, GetDEFAULTAllocator());
	env.pMasterClock = &m_timer;
	env.pResourceMgr = &m_resMgr;
	env.pInput = &m_input;
	env.pManifest = &m_manifest;

	m_resMgr.initialize();

	m_manifest.initialize();
	ObjectLibrary::instance()->initialize();

	m_pFrame = new SceneEditorFrame(
		"Scene Editor", 
		wxPoint(50, 50), 
		wxSize(1024, 768));
	m_pFrame->Maximize(true);
	m_pFrame->Show(true);
	SetTopWindow(m_pFrame);
	
	if (!m_pFrame->initGfx())
		return false;

	if (!Physics::initialize() && Animation::initialize())
	{
		return false;
	}

	m_pUI = COS_NEW UserInterface(m_pFrame);
	m_pController = COS_NEW Controller(
		env.pInput, m_pFrame->getMainView(), m_pUI);
	m_pFrame->setMessageBus(m_pController);

	return true;
}
//---------------------------------------------------------------------------
void SceneEditorApp::OnEndSession(wxCloseEvent& /*evt*/)
{
	OnExit();
}
//---------------------------------------------------------------------------
int SceneEditorApp::OnExit()
{
	delete m_pController;
	delete m_pUI;

	if (m_pData)
		delete m_pData;

	m_pRend->destroy();
	GfxRenderer::deallocate(m_pRend);
	Environment::get().pResourceMgr->destroy();
	Animation::shutdown();
	Physics::shutdown();
	ObjectLibrary::instance()->destroy();

#if defined(COS_OPTION_MEMPROFILE)
	AllocationTracker::getInstance()->PrintUsageByFile(m_logStrm);
#endif

	m_logStrm.close();
	return 0;
}
//---------------------------------------------------------------------------
int SceneEditorApp::OnRun()
{
	try
	{
		return wxApp::OnRun();
	}
	catch(...)
	{
	}

	return -1;
}
//---------------------------------------------------------------------------
void SceneEditorApp::OnUnhandledException()
{
}
//---------------------------------------------------------------------------
bool SceneEditorApp::DoFileOpen(wxString filename)
{
	wxBusyCursor hourglass;

	// clear SM first
	if (m_pData)
	{
		delete m_pData;
		m_pData = 0;
	}

	m_pData = COS_NEW SceneData(m_pController);
	if (!m_pData->load(filename))
	{
		delete m_pData;
		m_pData = 0;
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool SceneEditorApp::DoEntityOpen(const String& entityName)
{
	wxBusyCursor hourglass;

	// clear scene first
	if (m_pData)
	{
		delete m_pData;
		m_pData = 0;
	}

	m_pData = COS_NEW EntityData(m_pController);
	if (!m_pData->load(entityName))
	{
		delete m_pData;
		m_pData = 0;
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool SceneEditorApp::DoFileSave(wxString filename)
{
	if (m_pData)
	{
		wxBusyCursor hourglass;
		return m_pData->save(filename);
	}

	return true; // silently succeed
}
#if 0
//---------------------------------------------------------------------------
void SceneEditorApp::OnObjectSelected(Reflection::Object *pSelected)
{
	m_pSelected = pSelected;
}
//---------------------------------------------------------------------------
void SceneEditorApp::OnInputEvent(const InputEvent& evt)
{
	if (evt.evtType.type == InputEvent::MOUSE)
		handleMouse(evt.mouse);
	if (evt.evtType.type == InputEvent::KEYBOARD)
		handleKey(evt.key);
}
//---------------------------------------------------------------------------
void SceneEditorApp::handleMouse(const InputEvent::Mouse& evt)
{
	bool isButtonEvent = evt.btnsDown || evt.btnsPressed || evt.btnsReleased;

	if (evt.btnsDown == 0x00000001)
		m_bLMB = true;
	if (evt.btnsReleased == 0x00000001)
		m_bLMB = false;
	if (evt.btnsDown == 0x00000002)
		m_bRMB = true;
	if (evt.btnsReleased == 0x00000002)
	{
		m_bRMB = false;

		// also, cancel any current movements
		m_pFrame->handleCameraDolly(0);
		m_pFrame->handleCameraStrafe(0);
		m_pFrame->handleCameraLift(0);
	}

	// if no buttons are pressed, then this is a good time to see about what
	// manipulation operations might be available
	if (!isButtonEvent && shouldScanOperations())
	{
		m_bActiveOperation = m_pFrame->hasActiveOperation(evt.absX, evt.absY);
	}

	// mouse input is going either to the active camera controller in the 
	// active window/viewport, or it's going to the currently-selected object
	// in the active window/viewport. 
	if (shouldManipulate())
	{
		m_pFrame->handleManipulationMove(evt.absX, evt.absY, evt.relZ);
	}
	else if (shouldOrbit() || shouldRotate()) // both can make the camera move
	{
		m_pFrame->handleCameraRotate(evt.relX, evt.relY);

		if (evt.relZ)
			m_pFrame->handleCameraDolly(evt.relZ);
	}
	else if (shouldPick())
	{
		m_pFrame->pick(evt.absX, evt.absY);
	}
}
//---------------------------------------------------------------------------
void SceneEditorApp::handleKey(const InputEvent::Key& evt)
{
	switch (evt.keyCode)
	{
	case DIK_LCONTROL:
	case DIK_RCONTROL:
		m_bCtrl = evt.bKeyDown;
		break;
	case DIK_LALT:
	case DIK_RALT:
		m_bAlt = evt.bKeyDown;
		break;
	case DIK_LSHIFT:
	case DIK_RSHIFT:
		m_bShift = evt.bKeyDown;
		break;
	case DIK_F:
		if (evt.bKeyDown)
			m_pFrame->findSelectedInViewport();
		break;
	case DIK_SPACE:
		if (evt.bKeyDown && m_bCtrl)
			enterManipulationMode(MANIPULATE_SELECT);
		break;
	case DIK_T:
		if (evt.bKeyDown && m_bCtrl)
			enterManipulationMode(MANIPULATE_MOVE);
		break;
	case DIK_S:
		if (evt.bKeyDown && m_bCtrl)
			enterManipulationMode(MANIPULATE_SCALE);
		else if (m_bRMB)
			m_pFrame->handleCameraDolly(evt.bKeyDown ? -1 : 0);
		break;
	case DIK_R:
		if (evt.bKeyDown && m_bCtrl)
			enterManipulationMode(MANIPULATE_ROTATE);
		break;
	case DIK_W:
	case DIK_UP:
		if (m_bRMB)
			m_pFrame->handleCameraDolly(evt.bKeyDown ? 1 : 0);
		break;
	case DIK_DOWN:
		if (m_bRMB)
			m_pFrame->handleCameraDolly(evt.bKeyDown ? -1 : 0);
		break;
	case DIK_A:
	case DIK_LEFT:
		if (m_bRMB)
			m_pFrame->handleCameraStrafe(evt.bKeyDown ? -1 : 0);
		break;
	case DIK_D:
	case DIK_RIGHT:
		if (m_bRMB)
			m_pFrame->handleCameraStrafe(evt.bKeyDown ? 1 : 0);
		break;
	case DIK_DELETE:
		m_pFrame->deleteSelected();
		break;
	}
}
//---------------------------------------------------------------------------
void SceneEditorApp::enterManipulationMode(Manipulation manip)
{
	m_manip = manip;
	m_pFrame->setManipulation((SceneEditorFrame::Manipulation)manip);
}
//---------------------------------------------------------------------------
bool SceneEditorApp::shouldManipulate()
{
	/* we should manipulate if:
		(a) there is an active operation (implies something is selected)
		(b) the LMB is down
	*/
	return m_bActiveOperation && m_bLMB;
}
//---------------------------------------------------------------------------
bool SceneEditorApp::shouldOrbit()
{
	return (m_pSelected != 0) && m_bLMB && m_bAlt;
}
//---------------------------------------------------------------------------
bool SceneEditorApp::shouldRotate()
{
	return m_bRMB && !m_bAlt && !m_bCtrl;
}
//---------------------------------------------------------------------------
bool SceneEditorApp::shouldPick()
{
	return m_bLMB && !m_bActiveOperation;
}
//---------------------------------------------------------------------------
bool SceneEditorApp::shouldScanOperations()
{
	return m_pSelected != 0 && !m_bLMB;
}
#endif // 0
