/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "EntityChooser.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
EntityChooser::EntityChooser()
: wxDialog(
	0, 
	wxID_ANY, 
	wxT("Entity Chooser"), 
	wxDefaultPosition, 
	wxSize( 513,375 ), 
	wxCAPTION|wxDIALOG_NO_PARENT|wxRESIZE_BORDER|wxSYSTEM_MENU 
	)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_listBox2 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( -1,300 ), 0, NULL, wxLB_SINGLE|wxLB_SORT ); 
	bSizer1->Add( m_listBox2, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1OK->Disable();
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer1->Add( m_sdbSizer1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	m_listBox2->Connect( 
		wxEVT_COMMAND_LISTBOX_SELECTED, 
		wxCommandEventHandler( EntityChooser::OnSelectionChanged ), NULL, this );
	m_listBox2->Connect( 
		wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, 
		wxCommandEventHandler( EntityChooser::OnSelectionDblClick ), NULL, this );

	// fill in the list from the manifest entity entries
	const Manifest::NVP& nvp = Environment::get().pManifest->getDefinitions();
	for (Manifest::NVP::const_iterator it = nvp.begin(); it != nvp.end(); ++it)
	{
		String fullname(it->first);
		fullname += "/";
		fullname += it->second.val;
		m_listBox2->AppendString(wxString(fullname));
	}
}
//---------------------------------------------------------------------------
EntityChooser::~EntityChooser()
{
	// Disconnect Events
	m_listBox2->Disconnect( 
		wxEVT_COMMAND_LISTBOX_SELECTED, 
		wxCommandEventHandler( EntityChooser::OnSelectionChanged ), NULL, this );
	m_listBox2->Disconnect( 
		wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, 
		wxCommandEventHandler( EntityChooser::OnSelectionDblClick ), NULL, this );
}
//---------------------------------------------------------------------------
void EntityChooser::OnSelectionChanged( wxCommandEvent& event )
{
	m_name = event.GetString();

	if (m_name.length())
		m_sdbSizer1OK->Enable();
	else
		m_sdbSizer1OK->Disable();
}
//---------------------------------------------------------------------------
void EntityChooser::OnSelectionDblClick( wxCommandEvent& event )
{
	OnSelectionChanged(event);
	EndDialog(wxID_OK);
}
//---------------------------------------------------------------------------
const String& EntityChooser::getEntityName() const
{
	return m_name;
}
//---------------------------------------------------------------------------
void EntityChooser::OnAdd(wxCommandEvent& /*event*/)
{
}
//---------------------------------------------------------------------------
void EntityChooser::OnDelete(wxCommandEvent& /*event*/)
{
}
