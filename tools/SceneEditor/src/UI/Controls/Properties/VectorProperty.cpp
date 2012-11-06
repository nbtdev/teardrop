/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VectorProperty.h"
#include "FloatProperty.h"
#include "Math/Vector4.h"

using namespace Teardrop;
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
	Teardrop::Vector4& value,
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
