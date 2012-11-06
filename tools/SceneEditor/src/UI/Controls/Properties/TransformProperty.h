/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TRANSFORMPROPERTY_INCLUDED)
#define TRANSFORMPROPERTY_INCLUDED

/*
	wxPGProperty override for handling the special needs of 
	the Transform data type
*/

#include "wx/propgrid/props.h"
#include "Property.h"

class VectorProperty;
class QuaternionProperty;

class TransformProperty 
	:	 public wxPGProperty
		,public IProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(TransformProperty)
public:
	TransformProperty();
    TransformProperty( 
		Teardrop::Reflection::Object* pObj,
		const Teardrop::Reflection::PropertyDef* pProp,
		wxPropertyGridPage* pPage,
		const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
    virtual ~TransformProperty();

	void OnSetValue();

	// IProperty implementation
	void UpdateDisplay();
	void ValueChanged(const char* name, const char* val);

    WX_PG_DECLARE_BASIC_TYPE_METHODS()

protected:
	Teardrop::Reflection::Object* m_pObj;
	const Teardrop::Reflection::PropertyDef* m_pProp;
	Teardrop::Transform m_val;

	// this is temp
	VectorProperty* m_pos;
	QuaternionProperty* m_ori;
	VectorProperty* m_scl;
	wxPropertyGridPage* m_pPage;

	bool m_bInit;
};

#endif // TRANSFORMPROPERTY_INCLUDED
