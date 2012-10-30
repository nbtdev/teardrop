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
#include "TransformProperty.h"
#include "VectorProperty.h"
#include "QuaternionProperty.h"
#include "Math/include/Transform.h"

using namespace CoS;
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
	CoS::Reflection::Object* pObj,
	const CoS::Reflection::PropertyDef* pProp,
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
