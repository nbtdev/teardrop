///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MaterialFormFrame.h"

///////////////////////////////////////////////////////////////////////////

MaterialFormFrame::MaterialFormFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_sizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter2 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter2->SetSashGravity( 1 );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( MaterialFormFrame::m_splitter2OnIdle ), NULL, this );
	m_panel41 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter1 = new wxSplitterWindow( m_panel41, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->SetSashGravity( 1 );
	m_splitter1->SetSashSize( 4 );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( MaterialFormFrame::m_splitter1OnIdle ), NULL, this );
	m_panel3 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_editorPanel = new wxPanel( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer3->Add( m_editorPanel, 1, wxALL|wxEXPAND, 0 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cmdSave = new wxButton( m_panel3, wxID_CMD_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_cmdSave, 0, wxALL, 5 );
	
	m_cmdApply = new wxButton( m_panel3, wxID_CMD_APPLY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_cmdApply, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer3->Add( bSizer6, 0, wxALIGN_RIGHT, 5 );
	
	m_panel3->SetSizer( bSizer3 );
	m_panel3->Layout();
	bSizer3->Fit( m_panel3 );
	m_panel4 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_treeCtrl2 = new wxTreeCtrl( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer2->Add( m_treeCtrl2, 1, wxALL|wxEXPAND, 0 );
	
	m_panel4->SetSizer( bSizer2 );
	m_panel4->Layout();
	bSizer2->Fit( m_panel4 );
	m_splitter1->SplitVertically( m_panel3, m_panel4, 0 );
	bSizer5->Add( m_splitter1, 1, wxEXPAND, 5 );
	
	m_panel41->SetSizer( bSizer5 );
	m_panel41->Layout();
	bSizer5->Fit( m_panel41 );
	m_panel5 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );
	
	m_pLogWindow = new wxTextCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL|wxSUNKEN_BORDER );
	bSizer61->Add( m_pLogWindow, 1, wxALL|wxEXPAND, 0 );
	
	m_panel5->SetSizer( bSizer61 );
	m_panel5->Layout();
	bSizer61->Fit( m_panel5 );
	m_splitter2->SplitHorizontally( m_panel41, m_panel5, 0 );
	m_sizer1->Add( m_splitter2, 1, wxEXPAND, 5 );
	
	this->SetSizer( m_sizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MaterialFormFrame::OnClose ) );
}

MaterialFormFrame::~MaterialFormFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MaterialFormFrame::OnClose ) );
}
