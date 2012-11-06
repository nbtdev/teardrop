/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "QuaternionProperty.h"
#include "FloatProperty.h"
#include "Math/Quaternion.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(QuaternionProperty,wxStringProperty,
                               wxString,const wxString&,TextCtrl)
//---------------------------------------------------------------------------
static void toString(/*out*/String& str, /*in*/const Quaternion& q)
{
	char buf[256];
	sprintf_s(buf, 256, "(%0.3f,%0.3f,%0.3f,%0.3f)", q.w, q.x, q.y, q.z);
	str = buf;
}
//---------------------------------------------------------------------------
static Quaternion s_tmp;
QuaternionProperty::QuaternionProperty()
: wxStringProperty(wxPG_LABEL, wxPG_LABEL),
m_val(s_tmp)
{
}
//---------------------------------------------------------------------------
QuaternionProperty::QuaternionProperty(
	Teardrop::Quaternion& value,
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
	if (pPage && !pid)
		pid = pPage->Append(this);

	wxASSERT(pid && pPage);

	// make the sub-props in the grid
	wxPGId elemId = pPage->AppendIn(pid, this);
	m_w = new FloatProperty(m_val.w, pPage, elemId, "W");
	pPage->AppendIn(elemId, m_w);
	m_x = new FloatProperty(m_val.x, pPage, elemId, "X");
	pPage->AppendIn(elemId, m_x);
	m_y = new FloatProperty(m_val.y, pPage, elemId, "Y");
	pPage->AppendIn(elemId, m_y);
	m_z = new FloatProperty(m_val.z, pPage, elemId, "Z");
	pPage->AppendIn(elemId, m_z);
}
//---------------------------------------------------------------------------
QuaternionProperty::~QuaternionProperty()
{
}
//---------------------------------------------------------------------------
bool QuaternionProperty::StringToValue( 
	wxVariant& /*variant*/, 
	const wxString& /*text*/, 
	int /*argFlags*/ ) const
{
	return true;
}
//---------------------------------------------------------------------------
wxString QuaternionProperty::GetValueAsString( int ) const
{
	String str;
	GetValue();
	toString(str, m_val);
	return wxString(str);
}
//---------------------------------------------------------------------------
void QuaternionProperty::UpdateDisplay()
{
	RefreshEditor();
}
//---------------------------------------------------------------------------
void QuaternionProperty::ValueChanged(const char* name, const char* val)
{
	m_w->ValueChanged(name, val);
	m_x->ValueChanged(name, val);
	m_y->ValueChanged(name, val);
	m_z->ValueChanged(name, val);
}
//---------------------------------------------------------------------------
void QuaternionProperty::OnSetValue()
{
	wxStringProperty::OnSetValue();

	// notify parent, if present
	wxPGProperty* pParent = GetParent();
	if (pParent)
	{
		pParent->OnSetValue();
	}
}
