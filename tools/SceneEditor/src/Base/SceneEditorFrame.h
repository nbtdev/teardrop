/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SCENEEDITORFRAME_INCLUDED)
#define SCENEEDITORFRAME_INCLUDED

#include "FastDelegate.h"

namespace Teardrop
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
		const Teardrop::String& title, 
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
	Teardrop::Reflection::Object* getSelectedTemplate();
	bool handleInputEvent(const Teardrop::InputEvent& evt);

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
	
	Teardrop::String					m_title;
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
