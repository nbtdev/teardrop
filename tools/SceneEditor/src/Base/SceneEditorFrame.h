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

#if !defined(SCENEEDITORFRAME_INCLUDED)
#define SCENEEDITORFRAME_INCLUDED

#include "FastDelegate.h"

namespace CoS
{
	namespace Reflection
	{
		class Object;
	}

	union InputEvent;
}

class StatusBar;
class Toolbar;
class PropertyGridControl;
class ObjectTreeView;
class RenderWindow;
class View;
class MessageBus;
struct ContextMenu;
class MaterialForm;

class SceneEditorFrame 
	:	 public wxFrame
{
public:

	void m_splitter3OnIdle( wxIdleEvent& )
	{
		m_splitter3->SetSashPosition( 1082 );
		m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( SceneEditorFrame::m_splitter3OnIdle ), NULL, this );
	}

	void m_splitter5OnIdle( wxIdleEvent& )
	{
		m_splitter5->SetSashPosition( 0 );
		m_splitter5->Disconnect( wxEVT_IDLE, wxIdleEventHandler( SceneEditorFrame::m_splitter5OnIdle ), NULL, this );
	}

	enum
	{
		ID_Quit = 1,
		ID_About,
		ID_File_Open,
		ID_Entity_Open,
		ID_File_Save,

		ID_View_MaterialEditor,
		ID_View_AnimationControls,

		ID_Tools_Select,
		ID_Tools_Move,
		ID_Tools_Rotate,
		ID_Tools_Scale,
		ID_Tools_Place,
	};

	SceneEditorFrame(
		const CoS::String& title, 
		const wxPoint& pos, 
		const wxSize& size);
	virtual ~SceneEditorFrame();

	bool initGfx();
	void setMessageBus(MessageBus* pMsgBus);
	View* getMainView();
	StatusBar* getStatusBar();
	Toolbar* getToolBar();
	PropertyGridControl* getPropGrid();
	ObjectTreeView* getObjectTreeView();
	void showContextMenu(ContextMenu* pMenuDef, int x, int y);
	void showTemplateList(bool bShow);
	void showMaterialEditor(bool bShow);
	void showAnimationControls(bool bShow);
	CoS::Reflection::Object* getSelectedTemplate();
	bool handleInputEvent(const CoS::InputEvent& evt);

	func::delegate0<> FrameClosing;

private:
	DECLARE_EVENT_TABLE()
	void OnQuit(wxCommandEvent& evt);
	void OnAbout(wxCommandEvent& evt);
	void OnFileOpen(wxCommandEvent& evt);
	void OnFileSave(wxCommandEvent& evt);
	void OnEntityOpen(wxCommandEvent& evt);
	void OnViewMaterialEditor(wxCommandEvent& evt);
	void OnViewAnimationControls(wxCommandEvent& evt);
	void OnAnyMenu(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);
	void OnWindowActivate(wxActivateEvent& evt);
	
	CoS::String					m_title;
	ObjectTreeView*				m_pTreeView;
	PropertyGridControl*		m_pPropGrid; 
	Toolbar*					m_pToolBar;
	StatusBar*					m_pStatusBar;
	MessageBus*					m_pMessageBus;
	MaterialForm*				m_pMaterialForm;

	wxListBox* m_pTemplateList;
	wxSplitterWindow* m_splitter3;
	wxPanel* m_pPanel;
	RenderWindow* m_pRenderPanel;
	wxPanel* m_panel12;
	wxSplitterWindow* m_splitter5;
	wxPanel* m_panel13;
	wxPanel* m_panel14;
	wxMenuBar* m_menubar1;
	wxMenu* m_menu1;

	void* m_pContextData; // used for popup menus
};

#endif // SCENEEDITORFRAME_INCLUDED
