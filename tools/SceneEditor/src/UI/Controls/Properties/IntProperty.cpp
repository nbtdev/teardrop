/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "IntProperty.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(IntProperty,wxIntProperty,
                               wxString,const wxString&,TextCtrl)
//---------------------------------------------------------------------------
static int s_dummy = 0;
IntProperty::IntProperty()
: wxIntProperty(wxPG_LABEL, wxPG_LABEL),
m_val(s_dummy), m_bUserChanged(1)
{
}
//---------------------------------------------------------------------------
IntProperty::IntProperty(
	int& value,
	wxPropertyGridPage* pPage,
	wxPGId /*pid*/,
	const wxString& label, 
	const wxString& name)
:	wxIntProperty(label, name, value),
	m_val(value), m_bUserChanged(1)
{
	m_pPage = pPage;
}
//---------------------------------------------------------------------------
IntProperty::~IntProperty()
{
}
//---------------------------------------------------------------------------
bool IntProperty::StringToValue( 
	wxVariant& variant, 
	const wxString& text, 
	int /*argFlags*/ ) const
{
	long val;
	StringUtil::fromString(text, val);
	variant = val;
	return true;
}
//---------------------------------------------------------------------------
wxString IntProperty::GetValueAsString( int ) const
{
	String str;
	StringUtil::toString(m_val, str);
	return wxString(str);
}
//---------------------------------------------------------------------------
void IntProperty::UpdateDisplay()
{
	SetValue(m_val);
}
//---------------------------------------------------------------------------
void IntProperty::ValueChanged(const char* /*name*/, const char* /*val*/)
{
	m_bUserChanged = 0;
	SetValue(m_val);
}
//---------------------------------------------------------------------------
void IntProperty::OnSetValue() // called by wx prop framework
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

	m_val = GetValue().GetInteger();

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
