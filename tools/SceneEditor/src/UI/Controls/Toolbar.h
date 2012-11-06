/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TOOLBAR_INCLUDED)
#define TOOLBAR_INCLUDED

#include "FastDelegate.h"
#include "Util/_String.h"
#include <wx/toolBar.h>
#include <wx/combobox.h>
#include <vector>

class Tool;

/*
	Wrapper class for the underlying windowing system toolbar implementation;
	it's OK if it exposes whatever the underlying system is, the rest of the 
	editor app doesn't deal with those available APIs
*/

class Toolbar : 
	public wxToolBar
{
	wxChoice* m_pToolsetChooser;
	size_t m_currentToolset;

public:
	const static size_t INVALID_TOOLSET = size_t(-1);

	struct Entry
	{
		Teardrop::String text;
		Teardrop::String bitmap;
		Teardrop::String desc;
		Tool* tool;

		Entry()
		{
			tool = 0;
		}

		Entry(const Entry& other)
		{
			*this = other;
		}

		Entry& operator=(const Entry& other)
		{
			text = other.text;
			bitmap = other.bitmap;
			desc = other.desc;
			tool = other.tool;
			return *this;
		}
	};

	Toolbar(wxWindow* pParent);
	~Toolbar();

	// adds a new toolset, returns the toolset ID
	size_t addToolset(const Teardrop::String& name, const Tool** ppTools, size_t numTools);
	// set the current toolbar to a particular strip ID
	void setToolset(size_t id);

	// event raised when the user selects a different toolset 
	func::delegate1<size_t> ToolsetChanged;
	// event raised when the user selects a tool from the current toolset
	func::delegate2<Tool*, size_t> ToolChanged;

private:
	DECLARE_EVENT_TABLE()
	void OnToolsetChanged(wxCommandEvent& evt);
	void OnToolSelected(wxCommandEvent& evt);

	typedef std::vector<Entry> Toolset;
	typedef std::vector<Toolset> Toolsets;
	Toolsets m_toolsets;
};

#endif // TOOLBAR_INCLUDED
