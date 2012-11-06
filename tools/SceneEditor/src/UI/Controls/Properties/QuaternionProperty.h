/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QUATERNIONPROPERTY_INCLUDED)
#define QUATERNIONPROPERTY_INCLUDED

/*
	wxPGProperty override for handling the special needs of 
	the Vector4 data type -- note that this property only
	displays and manages the x, y and z components.
*/

#include "wx/propgrid/props.h"
#include "Property.h"

class FloatProperty;

class QuaternionProperty 
	:	 public wxStringProperty
		,public IProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(QuaternionProperty)
public:
	QuaternionProperty();
    QuaternionProperty( 
		Teardrop::Quaternion& value,
		wxPropertyGridPage* pPage = 0,
		wxPGId pid = 0,
		const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
    virtual ~QuaternionProperty();

	void OnSetValue();

	// IProperty implementation
	void UpdateDisplay();
	void ValueChanged(const char* name, const char* val);

    WX_PG_DECLARE_BASIC_TYPE_METHODS()

protected:
	Teardrop::Quaternion& m_val; // reference on purpose
	FloatProperty* m_w;
	FloatProperty* m_x;
	FloatProperty* m_y;
	FloatProperty* m_z;
	wxPropertyGridPage* m_pPage;
};

#endif // QUATERNIONPROPERTY_INCLUDED
