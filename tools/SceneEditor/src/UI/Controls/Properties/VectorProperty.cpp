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
#include "VectorProperty.h"
#include "FloatProperty.h"
#include "Math/include/Vector4.h"

using namespace CoS;
//---------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(VectorProperty,wxStringProperty,
                               wxString,const wxString&,TextCtrl)
//---------------------------------------------------------------------------
static void toString(/*out*/String& str, /*in*/const Vector4& vec)
{
	char buf[256];
	sprintf_s(buf, 256, "(%0.3f,%0.3f,%0.3f)", vec.x, vec.y, vec.z);
	str = buf;
}
//---------------------------------------------------------------------------
static Vector4 s_tmp;
VectorProperty::VectorProperty()
: wxStringProperty(wxPG_LABEL, wxPG_LABEL),
m_val(s_tmp)
{
}
//---------------------------------------------------------------------------
VectorProperty::VectorProperty(
	CoS::Vector4& value,
	wxPropertyGridPage* pPage,
	wxPGId pid,
	const wxString& label, 
	const wxString& name)
:	wxStringProperty(label, name),
	m_val(value)
{
	SetValue(GetValueAsString(0));
	SetFlag(wxPG_PROP_DISABLED);

	m_pPage = pPage;
	wxPGId elemId;
	if (pPage)
	{
		if (pid)
		{
			elemId = pPage->AppendIn(pid, this);
		}
		else
		{
			elemId = pPage->Append(this);
		}

		// make the sub-props in the grid
		m_x = new FloatProperty(m_val.x, pPage, elemId, "X");
		pPage->AppendIn(elemId, m_x);
		m_y = new FloatProperty(m_val.y, pPage, elemId, "Y");
		pPage->AppendIn(elemId, m_y);
		m_z = new FloatProperty(m_val.z, pPage, elemId, "Z");
		pPage->AppendIn(elemId, m_z);
	}
}
//---------------------------------------------------------------------------
VectorProperty::~VectorProperty()
{
}
//---------------------------------------------------------------------------
bool VectorProperty::StringToValue( 
	wxVariant& /*variant*/, 
	const wxString& /*text*/, 
	int /*argFlags*/ ) const
{
	return true;
}
//---------------------------------------------------------------------------
wxString VectorProperty::GetValueAsString( int ) const
{
	String str;
	toString(str, m_val);
	return wxString(str);
}
//---------------------------------------------------------------------------
void VectorProperty::UpdateDisplay()
{
	RefreshEditor();
}
//---------------------------------------------------------------------------
void VectorProperty::ValueChanged(const char* name, const char* val)
{
	m_x->ValueChanged(name, val);
	m_y->ValueChanged(name, val);
	m_z->ValueChanged(name, val);
}
//---------------------------------------------------------------------------
void VectorProperty::OnSetValue()
{
	wxStringProperty::OnSetValue();

	// notify parent, if present
	wxPGProperty* pParent = GetParent();
	if (pParent)
	{
		pParent->OnSetValue();
	}
}
