/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "TransformProperty.h"
#include "VectorProperty.h"
#include "QuaternionProperty.h"
#include "Math/Transform.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(TransformProperty,wxPGProperty,
                               wxString,const wxString&,TextCtrl)
//---------------------------------------------------------------------------
TransformProperty::TransformProperty()
: wxPGProperty(wxPG_LABEL, wxPG_LABEL), m_bInit(false)
{
}
//---------------------------------------------------------------------------
TransformProperty::TransformProperty(
	Teardrop::Reflection::Object* pObj,
	const Teardrop::Reflection::PropertyDef* pProp,
	wxPropertyGridPage* pPage,
	const wxString& label, 
	const wxString& name)
:	wxPGProperty(label, name),
	m_pObj(pObj), m_pProp(pProp), m_bInit(false)
{
	SetValue(GetValueAsString(0));
	SetFlag(wxPG_PROP_DISABLED);

	wxPGId pid = pPage->Append(this);
	m_pPage = pPage;

	// make the sub-props in the grid -- these are automatically
	// deleted when the containing grid page is cleared/deleted
	pProp->getData(pObj, &m_val);

	m_pos = new VectorProperty(
		m_val.trans,
		pPage,
		pid,
		"Position");

	m_ori = new QuaternionProperty(
		m_val.rot,
		pPage,
		pid,
		"Orientation");

	m_scl = new VectorProperty(
		m_val.scale,
		pPage,
		pid,
		"Scale");

	m_bInit = true;
}
//---------------------------------------------------------------------------
TransformProperty::~TransformProperty()
{
}
//---------------------------------------------------------------------------
bool TransformProperty::StringToValue( 
	wxVariant& /*variant*/, 
	const wxString& /*text*/, 
	int /*argFlags*/ ) const
{
	return true;
}
//---------------------------------------------------------------------------
wxString TransformProperty::GetValueAsString( int ) const
{
	String str;
	StringUtil::toString(m_val, str);
	return wxString(str);
}
//---------------------------------------------------------------------------
void TransformProperty::UpdateDisplay()
{
	m_pos->UpdateDisplay();
	m_ori->UpdateDisplay();
	m_scl->UpdateDisplay();
	//SetValue(GetValueAsString());
	RefreshEditor();
	m_pPage->RefreshGrid();
}
//---------------------------------------------------------------------------
void TransformProperty::ValueChanged(const char* name, const char* val)
{
	m_pProp->getData(m_pObj, &m_val);
	m_pos->ValueChanged(name, val);
	m_ori->ValueChanged(name, val);
	m_scl->ValueChanged(name, val);
}
//---------------------------------------------------------------------------
void TransformProperty::OnSetValue()
{
	// one of our nested values changed, set the transform to notify the world
	if (m_bInit)
		m_pProp->setData(m_pObj, &m_val);
}
