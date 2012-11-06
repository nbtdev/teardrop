/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "FloatProperty.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(FloatProperty,wxFloatProperty,
                               wxString,const wxString&,TextCtrl)
//---------------------------------------------------------------------------
static float s_dummy = 0;
FloatProperty::FloatProperty()
: wxFloatProperty(wxPG_LABEL, wxPG_LABEL),
m_val(s_dummy), m_bUserChanged(1)
{
}
//---------------------------------------------------------------------------
FloatProperty::FloatProperty(
	float& value,
	wxPropertyGridPage* pPage,
	wxPGId /*pid*/,
	const wxString& label, 
	const wxString& name)
:	wxFloatProperty(label, name, value),
	m_val(value), m_bUserChanged(1)
{
	m_pPage = pPage;
}
//---------------------------------------------------------------------------
FloatProperty::~FloatProperty()
{
}
//---------------------------------------------------------------------------
bool FloatProperty::StringToValue( 
	wxVariant& variant, 
	const wxString& text, 
	int /*argFlags*/ ) const
{
	float val;
	StringUtil::fromString(text, val);
	variant = val;
	return true;
}
//---------------------------------------------------------------------------
wxString FloatProperty::GetValueAsString( int ) const
{
	String str;
	StringUtil::toString(m_val, str);
	return wxString(str);
}
//---------------------------------------------------------------------------
void FloatProperty::UpdateDisplay()
{
	RefreshEditor();
}
//---------------------------------------------------------------------------
void FloatProperty::ValueChanged(const char* /*name*/, const char* /*val*/)
{
	m_bUserChanged = 0;
	SetValue(m_val);
}
//---------------------------------------------------------------------------
void FloatProperty::OnSetValue()
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

	m_val = (float)GetValue().GetReal();

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
