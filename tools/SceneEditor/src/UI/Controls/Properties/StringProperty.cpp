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
#include "StringProperty.h"

using namespace CoS;
//---------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(StringProperty,wxStringProperty,
                               wxString,const wxString&,TextCtrl)
//---------------------------------------------------------------------------
static String s_dummy;
StringProperty::StringProperty()
: wxStringProperty(wxPG_LABEL, wxPG_LABEL),
m_val(s_dummy), m_bUserChanged(1)
{
}
//---------------------------------------------------------------------------
StringProperty::StringProperty(
	String& value,
	wxPropertyGridPage* pPage,
	wxPGId /*pid*/,
	const wxString& label, 
	const wxString& name)
:	wxStringProperty(label, name, (const char*)value),
	m_val(value), m_bUserChanged(1)
{
	m_pPage = pPage;
}
//---------------------------------------------------------------------------
StringProperty::~StringProperty()
{
}
//---------------------------------------------------------------------------
bool StringProperty::StringToValue( 
	wxVariant& variant, 
	const wxString& text, 
	int /*argFlags*/ ) const
{
	variant = text;
	return true;
}
//---------------------------------------------------------------------------
wxString StringProperty::GetValueAsString( int ) const
{
	return wxString(m_val);
}
//---------------------------------------------------------------------------
void StringProperty::UpdateDisplay()
{
	SetValue((const char*)m_val);
}
//---------------------------------------------------------------------------
void StringProperty::ValueChanged(const char* /*name*/, const char* /*val*/)
{
	m_bUserChanged = 0;
	SetValue((const char*)m_val);
}
//---------------------------------------------------------------------------
void StringProperty::OnSetValue() // called by wx prop framework
{
	/*
		We can be here as a result of two things:
		1) the user made a change in the editor for this property
		2) code changed the property value on the object and we are here via 
		   ValueChanged()

		For (1) the change comes from the wxPGProperty internals; for (2) it 
		comes from there, but through ValueChanged, one of our methods. So the
		only way to know whether we want to propagate this notice is if the 
		m_bUserChanged flag is true (ValueChanged will set it to false). If we
		propagate this when it was sent via code, it will recursively overflow the
		stack. Either way, when done, set it to true.
	*/

	m_val = GetValue().GetString();

	if (m_bUserChanged)
	{
		// notify parent, if present
		wxPGProperty* pParent = GetParent();
		if (pParent)
		{
			pParent->OnSetValue();
		}
	}

	m_bUserChanged = 1;
}
