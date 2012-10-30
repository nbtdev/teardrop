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
#include "SceneEditorFrame.h"
#include "SceneEditorApp.h"
#include "RenderWindow.h"
#include "CoreInterface.h"
#include "Controller.h"
#include "MessageBus.h"
#include "UI/Controls/PropertyGrid.h"
#include "UI/Controls/ObjectTreeView.h"
#include "UI/Controls/ToolBar.h"
#include "UI/Controls/StatusBar.h"
#include "UI/ContextMenu.h"
#include "Forms/SceneChooser.h"
#include "Forms/EntityChooser.h"
#include "Forms/MaterialForm.h"
#include "Game/include/Prop.h"
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
wxImage s_toolbarIcons[4];
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(SceneEditorFrame, wxFrame)
	EVT_MENU(ID_Quit, SceneEditorFrame::OnQuit)
	EVT_MENU(ID_About, SceneEditorFrame::OnAbout)
	EVT_MENU(ID_File_Open, SceneEditorFrame::OnFileOpen)
	EVT_MENU(ID_Entity_Open, SceneEditorFrame::OnEntityOpen)
	EVT_MENU(ID_File_Save, SceneEditorFrame::OnFileSave)
	EVT_MENU(ID_View_MaterialEditor, SceneEditorFrame::OnViewMaterialEditor)
	EVT_MENU(ID_View_AnimationControls, SceneEditorFrame::OnViewAnimationControls)
	EVT_MENU(wxID_ANY, SceneEditorFrame::OnAnyMenu)
	EVT_CLOSE(SceneEditorFrame::OnClose)
	EVT_ACTIVATE(SceneEditorFrame::OnWindowActivate)
	//EVT_TOOL(SceneEditorFrame::ID_Tools_Select, SceneEditorFrame::OnToolSelect)
	//EVT_TOOL(SceneEditorFrame::ID_Tools_Move, SceneEditorFrame::OnToolMove)
	//EVT_TOOL(SceneEditorFrame::ID_Tools_Rotate, SceneEditorFrame::OnToolRotate)
	//EVT_TOOL(SceneEditorFrame::ID_Tools_Scale, SceneEditorFrame::OnToolScale)
	//EVT_TOOL(SceneEditorFrame::ID_Tools_Place, SceneEditorFrame::OnToolPlaceObject)
END_EVENT_TABLE()
//---------------------------------------------------------------------------
SceneEditorFrame::SceneEditorFrame(
	const String& title, 
	const wxPoint& pos, 
	const wxSize& size)
	:	wxFrame((wxFrame *)NULL, -1, (const char*)title, pos, size)
{
#if 0
	m_pScene = 0;
	m_pManip = 0;

	for (int i=0; i<MAX_RENDER_WINDOWS; ++i)
	{
		// create a perspective cam
		m_pCam[i] = new GfxCamera;
		m_pCam[i]->initialize(Environment::get());
		m_pCam[i]->setNearClip(1);
		m_pCam[i]->setFarClip(10000);
		m_pCam[i]->setAspect(1400.f / 900.f);
		m_pCam[i]->setFovY(MathUtil::PI / 4.f);

		m_pControllerMgr[i] = COS_NEW CameraControllerManager(m_pCam[i]);

		setCameraTarget(i, 0);
	}
#endif // 0

#if 0
	m_title = title; 
	wxMenu *menuFile = new wxMenu;
	wxMenu *menuHelp = new wxMenu;

	menuFile->Append( ID_File_Open, "&Open..." );
	menuFile->AppendSeparator();
	menuFile->Append( ID_Quit, "E&xit" );
	menuHelp->Append( ID_About, "&About..." );

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuHelp, "&Help" );

	SetMenuBar( menuBar );

	CreateStatusBar();
	SetStatusText( "" );

	// temp -- we are getting into the need for an actual UI manager now...
	s_toolbarIcons[0].LoadFile("icons/select.ico");
	s_toolbarIcons[1].LoadFile("icons/move.ico");
	s_toolbarIcons[2].LoadFile("icons/rotate.ico");
	s_toolbarIcons[3].LoadFile("icons/scale.ico");

	m_pToolBar = new wxToolBar(this, wxID_ANY);
	m_pToolBar->AddCheckTool(
		ID_Tools_Select, "Select", wxBitmap(s_toolbarIcons[0]), wxNullBitmap,
		"Select Tool", "Select an object in the scene for manipulation");
	m_pToolBar->AddCheckTool(
		ID_Tools_Move, "Move", wxBitmap(s_toolbarIcons[1]), wxNullBitmap,
		"Move Tool", "Move selected object in the scene");
	m_pToolBar->AddCheckTool(
		ID_Tools_Rotate, "Rotate", wxBitmap(s_toolbarIcons[2]), wxNullBitmap,
		"Rotate Tool", "Rotate selected object in the scene");
	m_pToolBar->AddCheckTool(
		ID_Tools_Scale, "Scale", wxBitmap(s_toolbarIcons[3]), wxNullBitmap,
		"Scale Tool", "Scale selected object in the scene");
	m_pToolBar->Realize();
	// end temp

	m_pSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D);
	m_pSplitter2 = new wxSplitterWindow(m_pSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D);

	//m_pPanel = new wxPanel(m_pSplitter, 0, 0, 1400, 900);
	m_pPanel = new wxWindow(m_pSplitter, wxID_ANY, wxPoint(0,0), wxSize(1400, 900));
	m_pPropGrid = new PropertyGridControl(m_pSplitter2, this);
	m_pTreeView = new ObjectTreeView(m_pSplitter2, this);

	m_pSplitter->SplitVertically(m_pSplitter2, m_pPanel, 200);
	m_pSplitter->SetSashSize(4);

	m_pSplitter2->SplitHorizontally(m_pTreeView, m_pPropGrid, 500);
	m_pSplitter2->SetSashSize(4);


	Layout();
#endif 

	//m_pWindow[0] = 0;
	//m_manipMode = ObjectManip::MANIP_TRANSLATE;

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	
	m_pToolBar = new Toolbar( this );
	//wxComboBox* pCB = new wxComboBox(m_pToolBar, wxID_ANY);
	//m_pToolBar->SetToolBitmapSize( wxSize( 16,16 ) );
	//m_pToolBar->AddControl(pCB);
	//m_pToolBar->AddTool( ID_Tools_Select, wxT("Select"), wxBitmap( wxT("icons/select.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Select"), wxT("Select Tool") );
	//m_pToolBar->AddTool( ID_Tools_Move, wxT("Move"), wxBitmap( wxT("icons/move.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Move"), wxT("Move Tool") );
	//m_pToolBar->AddTool( ID_Tools_Rotate, wxT("Rotate"), wxBitmap( wxT("icons/rotate.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Rotate"), wxT("Rotate Tool") );
	//m_pToolBar->AddTool( ID_Tools_Scale, wxT("Scale"), wxBitmap( wxT("icons/scale.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Scale"), wxT("Scale Tool") );
	//m_pToolBar->AddSeparator();
	//m_pToolBar->AddTool( ID_Tools_Place, wxT("Place"), wxBitmap( wxT("icons/place.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Place"), wxT("Place Object Tool") );
	//m_pToolBar->Realize();
	
	bSizer11->Add( m_pToolBar, 0, wxEXPAND, 5 );
	
	m_splitter3 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter3->Connect( wxEVT_IDLE, wxIdleEventHandler( SceneEditorFrame::m_splitter3OnIdle ), NULL, this );
	m_pPanel = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	m_pRenderPanel = COS_NEW RenderWindow( m_pPanel );
	bSizer13->Add( m_pRenderPanel, 1, wxEXPAND | wxALL, 1 );
	
	m_pPanel->SetSizer( bSizer13 );
	m_pPanel->Layout();
	bSizer13->Fit( m_pPanel );
	m_panel12 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter5 = new wxSplitterWindow( m_panel12, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter5->Connect( wxEVT_IDLE, wxIdleEventHandler( SceneEditorFrame::m_splitter5OnIdle ), NULL, this );
	m_panel13 = new wxPanel( m_splitter5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_pTemplateList = new wxListBox(m_panel13, wxID_ANY, wxDefaultPosition, wxSize(-1, 120));
	bSizer14->Add( m_pTemplateList, 1, wxALL|wxEXPAND, 0 );
	m_pTemplateList->Hide();
	//m_pTreeView = new ObjectTreeView(m_panel13, this);
	m_pTreeView = new ObjectTreeView(m_panel13);
	bSizer14->Add( m_pTreeView, 1, wxALL|wxEXPAND, 0 );
	
	m_panel13->SetSizer( bSizer14 );
	m_panel13->Layout();
	bSizer14->Fit( m_panel13 );
	m_panel14 = new wxPanel( m_splitter5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_pPropGrid = new PropertyGridControl(m_panel14);
	bSizer15->Add( m_pPropGrid, 1, wxEXPAND | wxALL, 0 );
	
	m_panel14->SetSizer( bSizer15 );
	m_panel14->Layout();
	bSizer15->Fit( m_panel14 );
	m_splitter5->SplitHorizontally( m_panel13, m_panel14, 0 );
	bSizer12->Add( m_splitter5, 1, wxEXPAND, 5 );
	
	m_panel12->SetSizer( bSizer12 );
	m_panel12->Layout();
	bSizer12->Fit( m_panel12 );
	m_splitter3->SplitVertically( m_pPanel, m_panel12, 1082 );
	bSizer11->Add( m_splitter3, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer11 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, ID_File_Open, wxString( wxT("&Open Scene...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	m_menuItem1 = new wxMenuItem( m_menu1, ID_Entity_Open, wxString( wxT("&Open Entity...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	m_menuItem1 = new wxMenuItem( m_menu1, ID_File_Save, wxString( wxT("&Save Scene...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu1, ID_Quit, wxString( wxT("E&xit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem2 );
	
	m_menubar1->Append( m_menu1, wxT("&File") );

	wxMenu* viewMenu = new wxMenu;
	wxMenuItem* viewItem = new wxMenuItem( viewMenu, ID_View_MaterialEditor, wxString(wxT("&Material Editor...")), wxEmptyString, wxITEM_NORMAL);
	viewMenu->Append(viewItem);
	viewItem = new wxMenuItem( viewMenu, ID_View_AnimationControls, wxString(wxT("&Animation Controls...")), wxEmptyString, wxITEM_NORMAL);
	viewMenu->Append(viewItem);
	m_menubar1->Append(viewMenu, wxT("&View"));
	
	this->SetMenuBar( m_menubar1 );
	
	m_pStatusBar = new StatusBar(this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY ));

	Layout();

	// add templates to the template list
	Manifest::EntryList list;
	Environment::get().pManifest->getDefinitionsByCategory("Prop", list);
	for (Manifest::EntryList::iterator it = list.begin();
		it != list.end(); ++it)
	{
		int idx = m_pTemplateList->GetCount();
		m_pTemplateList->AppendString((const char*)it->val);

		// find the object template in the ObjectLibrary
		Reflection::Object* pObj = static_cast<Reflection::Object*>(
			ObjectLibrary::instance()->getByName(ObjectLibrary::PROP, (const char*)it->val));
		m_pTemplateList->SetClientData(idx, pObj);
	}

	m_pTemplateList->Disable();
	m_pMaterialForm = 0;
}
//---------------------------------------------------------------------------
SceneEditorFrame::~SceneEditorFrame()
{
	delete m_pMaterialForm;
	delete m_pStatusBar;
}
//---------------------------------------------------------------------------
void SceneEditorFrame::setMessageBus(MessageBus* pMsgBus)
{
	m_pMessageBus = pMsgBus;
	m_pMaterialForm = new MaterialForm;
}
//---------------------------------------------------------------------------
View* SceneEditorFrame::getMainView()
{
	return m_pRenderPanel;
}
//---------------------------------------------------------------------------
PropertyGridControl* SceneEditorFrame::getPropGrid()
{
	return m_pPropGrid;
}
//---------------------------------------------------------------------------
ObjectTreeView* SceneEditorFrame::getObjectTreeView()
{
	return m_pTreeView;
}
//---------------------------------------------------------------------------
StatusBar* SceneEditorFrame::getStatusBar()
{
	return m_pStatusBar;
}
//---------------------------------------------------------------------------
Toolbar* SceneEditorFrame::getToolBar()
{
	return m_pToolBar;
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnViewMaterialEditor(wxCommandEvent& /*evt*/)
{
	showMaterialEditor(true);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnViewAnimationControls(wxCommandEvent& /*evt*/)
{
	showAnimationControls(true);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::showMaterialEditor(bool bShow)
{
	m_pMaterialForm->Show(bShow);
	m_pMaterialForm->setObject(m_pPropGrid->getSelectedObject());
}
//---------------------------------------------------------------------------
void SceneEditorFrame::showAnimationControls(bool /*bShow*/)
{
}
//---------------------------------------------------------------------------
static void createMenu(wxMenu& menu, MenuEntry* pMenu)
{
	while (pMenu)
	{
		if (!pMenu->id && !pMenu->isSeparator)
			break;

		if (pMenu->subMenu)
		{
			wxMenu* subMenu = new wxMenu;
			createMenu(*subMenu, pMenu->subMenu);
			menu.AppendSubMenu(subMenu, pMenu->label);
		}
		else if (pMenu->isSeparator)
		{
			menu.AppendSeparator();
		}
		else
		{
			menu.Append(pMenu->id, pMenu->label, 0);
		}

		pMenu++;
	}
}
//---------------------------------------------------------------------------
void SceneEditorFrame::showContextMenu(ContextMenu* pMenuDef, int x, int y)
{
	if (!pMenuDef)
		return;

	m_pContextData = pMenuDef;
	wxMenu menu;
	createMenu(menu, pMenuDef->menu);
	PopupMenu(&menu, ClientToScreen(wxPoint(x, y)));
}
//---------------------------------------------------------------------------
Reflection::Object* SceneEditorFrame::getSelectedTemplate()
{
	int idx = m_pTemplateList->GetSelection();
	if (idx < 0)
		return 0;

	return static_cast<Reflection::Object*>(m_pTemplateList->GetClientData(idx));
}
//---------------------------------------------------------------------------
void SceneEditorFrame::showTemplateList(bool bShow)
{
	if (bShow)
		m_pTemplateList->Enable();
	else
		m_pTemplateList->Disable();

	m_pTemplateList->Show(bShow);
	m_panel13->Layout();
}
////---------------------------------------------------------------------------
//void SceneEditorFrame::setScene(Scene* pScene)
//{
//	m_pScene = pScene;
//
//	if (!pScene)
//	{
//		// then clear all of the controls 
//		resetAll();
//	}
//}
//---------------------------------------------------------------------------
//void SceneEditorFrame::OnResize(wxSizeEvent& evt)
//{
//	if (m_bIsInitialized)
//	{
//		wxRect r = m_pRenderPanel->GetClientRect();
//		m_pWindow[0]->resizeWindow(r.width, r.height);
//	}
//
//	wxFrame::OnSize(evt);
//}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnAnyMenu(wxCommandEvent& evt)
{
	size_t id = size_t(evt.GetId());
	m_pMessageBus->ContextMenuSelection(m_pContextData, id);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnClose(wxCloseEvent& /*event*/)
{
	//if (m_pManip)
	//{
	//	m_pManip->destroy();
	//	delete m_pManip;
	//	m_pManip = 0;
	//}

	//if (m_pScene)
	//{
	//	m_pScene->destroy();
	//	delete m_pScene;
	//	m_pScene = 0;
	//}

	FrameClosing();
	Destroy();
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnFileSave(wxCommandEvent& WXUNUSED(evt))
{
	//const String& currentScenePathname = wxGetApp().getCurrentScenePathname();
	//String sceneDir(
	//	currentScenePathname.substr(0, currentScenePathname.findLast('/')));
	//String sceneFile(
	//	currentScenePathname.substr(currentScenePathname.findLast('/')+1));

	wxFileDialog dlg(
		this, 
		"Save Scene File",
		wxEmptyString,//(const char*)sceneDir,
		wxEmptyString,//(const char*)sceneFile,
		"Scene File (*.xml)|*.xml|All Files (*.*)|*.*",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
		);

	if (dlg.ShowModal() == wxID_OK)
	{
		if (!wxGetApp().DoFileSave(dlg.GetPath()))
		{
			// TODO: better error message
			wxMessageBox("Could not save scene file", "Error saving file");
			return;
		}
	}
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnQuit(wxCommandEvent& WXUNUSED(evt))
{
	Close(true);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnAbout(wxCommandEvent& WXUNUSED(evt))
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
//---------------------------------------------------------------------------
bool SceneEditorFrame::initGfx()
{
	Refresh();

	Environment& env = Environment::get();
	GfxRenderConfig cfg;
	cfg.bNVPerfHUDAdapter = false;
	cfg.fullscreen = false;
	cfg.hWnd = m_pRenderPanel->GetHWND();
	cfg.pLogger = env.pLogger;
	cfg.vsync = false;

	wxSize sz = m_pRenderPanel->GetSize();
	cfg.height = sz.GetX();
	cfg.width = sz.GetY();

	if (env.pRenderer->initialize(cfg, false))
	{
		//m_pWindow[0] = env.pRenderer->getMainWindow();

		//while (!m_pWindow[0])
		//{
		//	Sleep(100);
		//	m_pWindow[0] = env.pRenderer->getMainWindow();
		//}

		//m_pVP[0] = m_pWindow[0]->addViewport(0, 0, 1, 1);
		//m_pVP[0]->enable(true);

		// init input
		//env.pInput->initialize(env, this->GetHWND());
		env.pInput->initialize(env, m_pRenderPanel->GetHWND());

		// create object manipulator/gizmo
		//m_pManip = new ObjectManip;
		//m_pManip->initialize();
		//m_pManip->setEnabled(true);
		//m_pManip->setMode(m_manipMode);
		//m_pManip->setCamera(m_pCam[0]);
		//m_pManip->setRenderWindow(m_pWindow[0]);
		//m_pManip->addListener(this);

		//env.pRenderer->swap(*m_pRenderStats);
		env.pResourceMgr->update();

		//m_bIsInitialized = 1;
		return true;
	}

	return false;
}
#if 0
//---------------------------------------------------------------------------
void SceneEditorFrame::renderFrame()
{
	if (!m_pScene)
		return;

	Environment& env = Environment::get();

	env.pMasterClock->update();
	if (env.pMasterClock->getElapsedTime() > 1)
	{
		env.pMasterClock->update();
		env.pMasterClock->saveElapsedTime();
	}

	float deltaT = env.pMasterClock->getElapsedTime();

	// gather input
	InputEvent event;
	while (env.pInput->getNextEvent(event))
	{
		m_pListener->OnInputEvent(event);
	}

	if (m_pPanel)
	{
		size_t ww = m_pWindow[0]->getWidth();
		size_t wh = m_pWindow[0]->getHeight();
		wxRect rect = m_pPanel->GetClientRect();
		float w = (float)rect.width;
		float h = (float)rect.height;
		float aspect = w / h;
		m_pCam[0]->setAspect(aspect);
	}

	for (int i=0; i<MAX_RENDER_WINDOWS; ++i)
	{
		m_pControllerMgr[i]->update(deltaT);
	}

	m_pManip->update(deltaT);

	// update scene
	m_pScene->update(deltaT);


	Vector2 sz(m_pVP[0]->getSize());
	m_pCam[0]->setAspect(sz.x / sz.y);

	env.pRenderer->setRenderTarget(m_pWindow[0]);
	env.pRenderer->beginFrame();
	env.pRenderer->setViewport(m_pVP[0]);
	env.pRenderer->setCamera(m_pCam[0]);

	// get the visible objects from the scene; if any, pass over to the renderer
	ZoneObjects objects;
	if (m_pScene->getVisibleObjects(*m_pCam[0], objects))
	{
		// go through the list and make another of things we need to render
		for(ZoneObjects::iterator it = objects.begin();
			it != objects.end(); ++it)
		{
			ZoneObject* pObj = const_cast<ZoneObject*>(*it);
			RenderComponent* pRend = 0;
			if (pObj->findComponents(
				RenderComponent::getClassDef(), (Component**)&pRend))
			{
				// update render component's affecting light list if needed. this
				// call will internally short-circuit if the object is not lit or
				// the light list doesn't need rebuilt
				pRend->updateLightList(m_pScene);
				env.pRenderer->queueForRendering(pRend->getMeshInstance());
			}
		}
	}

	if (m_pManip->isEnabled())
	{
		env.pRenderer->queueForRendering(m_pManip->getRenderable(0));
	}

	env.pRenderer->setAmbientLight(m_pScene->getAmbientLight());
	env.pRenderer->flush();

	// then submit a second pass for the gizmo's "hidden" bits
	if (m_pManip->isEnabled())
	{
		env.pRenderer->queueForRendering(m_pManip->getRenderable(1));
	}

	env.pRenderer->flush();
	env.pRenderer->endFrame();

	env.pRenderer->swap(*m_pRenderStats);

	// update physics visual debugger
	Physics::advanceDebugger(deltaT);
	env.pResourceMgr->update();
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnIdle(wxIdleEvent& event)
{
	renderFrame();
	event.RequestMore(true);
}
//---------------------------------------------------------------------------
static ObjectManip::ManipulationMode s_manipMap[] = 
{
	ObjectManip::MANIP_NONE,
	ObjectManip::MANIP_TRANSLATE,
	ObjectManip::MANIP_ROTATE,
	ObjectManip::MANIP_SCALE,
};
//---------------------------------------------------------------------------
void SceneEditorFrame::setManipulation(Manipulation manip)
{
	m_manipMode = s_manipMap[manip];
	m_pManip->setMode(m_manipMode);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::handleManipulationMove(int dx, int dy, int dz)
{
	m_pManip->manipulate(dx, dy, dz);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::pick(int x, int y)
{
	if (!m_pRenderPanel->IsMouseInWindow())
		return;

	// select object under the provided mouse coords
	m_pManip->pick(x, y);
}
//---------------------------------------------------------------------------
bool SceneEditorFrame::hasActiveOperation(int x, int y)
{
	// check with the manip/gizmo to scan for an active operation
	return m_pManip->hasActiveOperation(x, y);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::handleCameraRotate(int dx, int dy)
{
	m_pControllerMgr[0]->rotate(dx, dy);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::handleCameraDolly(int value)
{
	m_pControllerMgr[0]->dolly(value);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::handleCameraStrafe(int value)
{
	m_pControllerMgr[0]->strafe(value);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::handleCameraLift(int value)
{
	m_pControllerMgr[0]->lift(value);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::findSelectedInViewport()
{
}
#endif // 0
//---------------------------------------------------------------------------
void SceneEditorFrame::OnFileOpen(wxCommandEvent& /*evt*/)
{
	SceneChooser chooser;
	if (chooser.ShowModal() == wxID_OK)
	{
		String sceneName(chooser.getScenePath());
		sceneName += "/def.xml";

		if (!wxGetApp().DoFileOpen((const char*)sceneName))
		{
			// TODO: better error message
			wxMessageBox("Could not open scene file", "Error opening file");
			return;
		}

		//m_pTemplateList->Enable();

		//// clear any selections
		//m_pTreeView->selectObject(0);
		//m_pPropGrid->setSelectedObject(0);
	}
}
#if 0
//---------------------------------------------------------------------------
void SceneEditorFrame::resetAll()
{
	m_pTreeView->clear();
}
//---------------------------------------------------------------------------
ZoneData* SceneEditorFrame::addZone(Zone* pZone)
{
	ZoneData* pData = m_pTreeView->addZone(pZone);

	// add the standard categories
	addCategory(pData, "Objects");

	// yes, this means the last one added is where we start...
	setCurrentZone(pZone);

	return pData;
}
//---------------------------------------------------------------------------
void SceneEditorFrame::addCategory(ZoneData* pData, const char* category)
{
	m_pTreeView->addCategory(pData, category);
}
//---------------------------------------------------------------------------
SceneObject* SceneEditorFrame::addObject(ZoneObject* pObj, ZoneData* pZone)
{
	return m_pTreeView->addObject(pObj, pZone, "Objects");
}
//---------------------------------------------------------------------------
void SceneEditorFrame::setCurrentZone(Zone* pZone)
{
	if (m_pManip)
	{
		m_pManip->setZone(pZone);
	}
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnObjectChanged(ZoneObject* pObj)
{
	m_pTreeView->selectObject(pObj);
	TreeViewSelectionChanged(pObj);

	// filter out stuff we don't want in the prop grid
	if (pObj && pObj->getClassId() == TerrainPatch::Class)
		pObj = 0;

	m_pPropGrid->setSelectedObject(pObj);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::selectObject(SceneObject* pObj)
{
	m_pTreeView->selectObject(pObj->getObject());
}
//---------------------------------------------------------------------------
void SceneEditorFrame::TreeViewSelectionChanged(Reflection::Object *pObj)
{
	if (m_pListener)
	{
		m_pListener->OnObjectSelected(pObj);
	}

	// filter out stuff we don't want in the prop grid
	if (pObj && pObj->getDerivedClassDef() == TerrainPatch::getClassDef())
		pObj = 0;

	m_pPropGrid->setSelectedObject(pObj);

	if (pObj && pObj->getDerivedClassDef()->isA(ZoneObject::getClassDef()))
		m_pManip->setTarget(static_cast<ZoneObject*>(pObj));

	//setCameraTarget(0, pObj);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::setCameraTarget(size_t cam, ZoneObject *pObj)
{
	assert(cam < MAX_RENDER_WINDOWS);
	m_pControllerMgr[cam]->changeTarget(pObj);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::PropertyChanged(
	CoS::Reflection::Object* pObj,
	CoS::Reflection::PropertyDef* pProp)
{
	m_pTreeView->UpdateNodeForObject(pObj);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnToolSelect(wxCommandEvent& evt)
{
	setManipulation(MANIPULATE_SELECT);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnToolMove(wxCommandEvent& evt)
{
	setManipulation(MANIPULATE_MOVE);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnToolRotate(wxCommandEvent& evt)
{
	setManipulation(MANIPULATE_ROTATE);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnToolScale(wxCommandEvent& evt)
{
	setManipulation(MANIPULATE_SCALE);
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnToolPlaceObject(wxCommandEvent& evt)
{
	setManipulation(MANIPULATE_SELECT);
	if (evt.IsChecked())
	{
		m_pTemplateList->Show();
	}
	else
	{
		m_pTemplateList->Hide();
	}

	m_panel13->Layout();
}
//---------------------------------------------------------------------------
void SceneEditorFrame::deleteSelected()
{
	ZoneObject* pObj = m_pManip->getSelectedObject();
	if (pObj)
	{
		m_pScene->getCurrentZone()->removeObject(pObj);
		m_pTreeView->removeObject(pObj);
		m_pPropGrid->setSelectedObject(0);
		m_pManip->setTarget(0);
	}
}
#endif // 0
//---------------------------------------------------------------------------
void SceneEditorFrame::OnEntityOpen(wxCommandEvent& /*evt*/)
{
	EntityChooser chooser;
	if (chooser.ShowModal() == wxID_OK)
	{
		if (!wxGetApp().DoEntityOpen(chooser.getEntityName()))
		{
			// TODO: better error message
			wxMessageBox("Could not open entity file", "Error opening file");
			return;
		}
	}
}
//---------------------------------------------------------------------------
void SceneEditorFrame::OnWindowActivate(wxActivateEvent& evt)
{
	Controller* pController = CoreInterface::get().pController;
	if (pController)
		pController->activate(evt.GetActive());
}
