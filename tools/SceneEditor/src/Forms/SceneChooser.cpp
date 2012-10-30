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
#include "SceneChooser.h"

using namespace CoS;
//---------------------------------------------------------------------------
SceneChooser::SceneChooser()
: wxDialog(
	0, 
	wxID_ANY, 
	wxT("Scene Chooser"), 
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
		wxCommandEventHandler( SceneChooser::OnSelectionChanged ), NULL, this );
	m_listBox2->Connect( 
		wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, 
		wxCommandEventHandler( SceneChooser::OnSelectionDblClick ), NULL, this );

	// fill in the list from the manifest Scene entries
	Manifest::EntryList list;
	Environment::get().pManifest->getDefinitionsByCategory("Scene", list);

	for (Manifest::EntryList::iterator it = list.begin();
		it != list.end(); ++it)
	{
		m_listBox2->AppendString((const char*)it->val);
	}
}
//---------------------------------------------------------------------------
SceneChooser::~SceneChooser()
{
	// Disconnect Events
	m_listBox2->Disconnect( 
		wxEVT_COMMAND_LISTBOX_SELECTED, 
		wxCommandEventHandler( SceneChooser::OnSelectionChanged ), NULL, this );
	m_listBox2->Disconnect( 
		wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, 
		wxCommandEventHandler( SceneChooser::OnSelectionDblClick ), NULL, this );
}
//---------------------------------------------------------------------------
void SceneChooser::OnSelectionChanged( wxCommandEvent& event )
{
	m_name = event.GetString();
	m_path = String::EMPTY;

	// figure out the path to the scene file
	const Manifest::NVP& nvp = Environment::get().pManifest->getDefinitions();
	Manifest::NVP::const_iterator it = nvp.find((const char*)m_name);

	if (it != nvp.end())
	{
		m_path = Environment::get().pDataPath;
		m_path += it->second.val;
	}

	if (m_name.length() && m_path.length())
		m_sdbSizer1OK->Enable();
	else
		m_sdbSizer1OK->Disable();
}
//---------------------------------------------------------------------------
void SceneChooser::OnSelectionDblClick( wxCommandEvent& event )
{
	OnSelectionChanged(event);
	EndDialog(wxID_OK);
}
//---------------------------------------------------------------------------
const String& SceneChooser::getSceneName() const
{
	return m_name;
}
//---------------------------------------------------------------------------
const String& SceneChooser::getScenePath() const
{
	return m_path;
}
//---------------------------------------------------------------------------
void SceneChooser::OnAdd(wxCommandEvent& /*event*/)
{
}
//---------------------------------------------------------------------------
void SceneChooser::OnDelete(wxCommandEvent& /*event*/)
{
}
