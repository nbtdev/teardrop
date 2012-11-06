/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VECTORPROPERTY_INCLUDED)
#define VECTORPROPERTY_INCLUDED

/*
	wxPGProperty override for handling the special needs of 
	the Vector4 data type -- note that this property only
	displays and manages the x, y and z components.
*/

#include "wx/propgrid/props.h"
#include "Property.h"

class FloatProperty;

class VectorProperty 
	:	 public wxStringProperty
		,public IProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(VectorProperty)
public:
	VectorProperty();
    VectorProperty( 
		Teardrop::Vector4& value,
		wxPropertyGridPage* pPage = 0,
		wxPGId pid = 0,
		const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
    virtual ~VectorProperty();

	void OnSetValue();

	// IProperty implementation
	void UpdateDisplay();
	void ValueChanged(const char* name, const char* val);

    WX_PG_DECLARE_BASIC_TYPE_METHODS()

protected:
	Teardrop::Vector4& m_val; // reference on purpose
	FloatProperty* m_x;
	FloatProperty* m_y;
	FloatProperty* m_z;
	wxPropertyGridPage* m_pPage;
};

#endif // VECTORPROPERTY_INCLUDED
