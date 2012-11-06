/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ToolBar.h"
#include "Tools/Tool.h"
#include "Base/Attribute.h"
#include "Memory/Memory.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(Toolbar, wxToolBar)
	EVT_CHOICE(wxID_ANY, Toolbar::OnToolsetChanged)
	EVT_TOOL(wxID_ANY, Toolbar::OnToolSelected)
END_EVENT_TABLE()
//---------------------------------------------------------------------------
Toolbar::Toolbar(wxWindow* pParent)
: wxToolBar(
	pParent, 
	wxID_ANY, 
	wxDefaultPosition, 
	wxDefaultSize, 
	wxTB_DOCKABLE|wxTB_HORIZONTAL)
{
	m_pToolsetChooser = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0);
	SetToolBitmapSize( wxSize( 16,16 ) );
	AddControl(m_pToolsetChooser);
	//m_pToolBar->AddTool( ID_Tools_Select, wxT("Select"), wxBitmap( wxT("icons/select.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Select"), wxT("Select Tool") );
	//m_pToolBar->AddTool( ID_Tools_Move, wxT("Move"), wxBitmap( wxT("icons/move.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Move"), wxT("Move Tool") );
	//m_pToolBar->AddTool( ID_Tools_Rotate, wxT("Rotate"), wxBitmap( wxT("icons/rotate.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Rotate"), wxT("Rotate Tool") );
	//m_pToolBar->AddTool( ID_Tools_Scale, wxT("Scale"), wxBitmap( wxT("icons/scale.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Scale"), wxT("Scale Tool") );
	//m_pToolBar->AddSeparator();
	//m_pToolBar->AddTool( ID_Tools_Place, wxT("Place"), wxBitmap( wxT("icons/place.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Place"), wxT("Place Object Tool") );
	Realize();
	m_currentToolset = size_t(-1);
}
//---------------------------------------------------------------------------
Toolbar::~Toolbar()
{
}
//---------------------------------------------------------------------------
size_t Toolbar::addToolset(
	const Teardrop::String& name, 
	const Tool** ppTools, 
	size_t numTools)
{
	size_t id = m_toolsets.size();
	m_toolsets.push_back(Toolset());

	for (size_t i=0; i<numTools; ++i)
	{
		Entry ent;
		const Attribute* pAttr = ppTools[i]->getAttribute("name");
		if (pAttr)
			ent.text = pAttr->getValue();
		pAttr = ppTools[i]->getAttribute("description");
		if (pAttr)
			ent.desc = pAttr->getValue();
		pAttr = ppTools[i]->getAttribute("icon");
		if (pAttr)
			ent.bitmap = pAttr->getValue();

		// const-cast because we're just storing this here to give back to 
		// someone else who probably will use it to do something
		ent.tool = const_cast<Tool*>(ppTools[i]);

		m_toolsets[id].push_back(ent);
	}

	// add the toolset name to the dropdown
	m_pToolsetChooser->AppendString((const char*)name);

	return id;
}
//---------------------------------------------------------------------------
void Toolbar::setToolset(size_t id)
{
	if (id >= m_toolsets.size() || id == m_currentToolset)
		return;

	// remove all existing tools
	size_t numTools = GetToolsCount();

	// skip the first one as it is the dropdown and we don't want to lose it
	// also, deleting them changes the number of tools so iterating forward 
	// will run us past the end of the tools in the bar
	for (size_t i=numTools-1; i>0; --i)
	{
		this->DeleteToolByPos(i);
	}

	// add all of the tools in the indicated toolset
	Toolset& tools = m_toolsets[id];
	for (size_t i=0; i<tools.size(); ++i)
	{
		Entry& ent = tools[i];

		AddRadioTool(
			wxID_HIGHEST + i,
			wxT((const char*)ent.text),
			wxBitmap( wxT((const char*)ent.bitmap), wxBITMAP_TYPE_ANY ),
			wxNullBitmap,
			wxT((const char*)ent.text), 
			wxT((const char*)ent.desc)
			);
	}

	Realize();

	// set the dropdown to this index, in case this was called from outside
	m_pToolsetChooser->SetSelection(id);
	m_currentToolset = id;
}
//---------------------------------------------------------------------------
void Toolbar::OnToolsetChanged(wxCommandEvent& evt)
{
	ToolsetChanged(evt.GetInt());

	// change the toolbar (if needed)
	setToolset(evt.GetInt());

	// update the current toolset ID
	m_currentToolset = evt.GetInt();
}
//---------------------------------------------------------------------------
void Toolbar::OnToolSelected(wxCommandEvent& evt)
{
	if (m_currentToolset == size_t(-1))
		return;

	// just use the tool ID (minus the wx offset) to index the current 
	// toolset, and grab the Tool* from there
	size_t idx = evt.GetId() - wxID_HIGHEST;
	ToolChanged(
		m_toolsets[m_currentToolset][idx].tool, idx);
}
