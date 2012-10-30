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

#if !defined(TOOLBAR_INCLUDED)
#define TOOLBAR_INCLUDED

#include "FastDelegate.h"
#include "Util/include/_String.h"
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
		CoS::String text;
		CoS::String bitmap;
		CoS::String desc;
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
	size_t addToolset(const CoS::String& name, const Tool** ppTools, size_t numTools);
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
