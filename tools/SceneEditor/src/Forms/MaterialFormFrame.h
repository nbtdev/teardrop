///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MaterialFormFrame__
#define __MaterialFormFrame__

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MaterialFormFrame
///////////////////////////////////////////////////////////////////////////////
class MaterialFormFrame : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			wxID_CMD_SAVE = 1000,
			wxID_CMD_APPLY,
		};
		
		wxBoxSizer* m_sizer1;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel41;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel3;
		wxBoxSizer* bSizer3;
		wxPanel* m_editorPanel;
		wxButton* m_cmdSave;
		wxButton* m_cmdApply;
		wxPanel* m_panel4;
		wxTreeCtrl* m_treeCtrl2;
		wxPanel* m_panel5;
		wxTextCtrl* m_pLogWindow;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		
	
	public:
		MaterialFormFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Material"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 956,678 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MaterialFormFrame();
		void m_splitter2OnIdle( wxIdleEvent& )
		{
		m_splitter2->SetSashPosition( 0 );
		m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MaterialFormFrame::m_splitter2OnIdle ), NULL, this );
		}
		
		void m_splitter1OnIdle( wxIdleEvent& )
		{
		m_splitter1->SetSashPosition( 0 );
		m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MaterialFormFrame::m_splitter1OnIdle ), NULL, this );
		}
		
	
};

#endif //__MaterialFormFrame__
