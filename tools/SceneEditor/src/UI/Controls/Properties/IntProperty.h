/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INTPROPERTY_INCLUDED)
#define INTPROPERTY_INCLUDED

/*
	Wrapper/helper for the wxIntProperty type (really
	all we are doing is adding more methods to it, basically
	to expose the IProperty interface on it)
*/

#include "wx/propgrid/props.h"
#include "Property.h"

class IntProperty 
	:	 public wxIntProperty
		,public IProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(IntProperty)
public:
	IntProperty();
    IntProperty( 
		int& value,
		wxPropertyGridPage* pPage = 0,
		wxPGId pid = 0,
		const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
    virtual ~IntProperty();

	void OnSetValue();

	// IProperty implementation
	void UpdateDisplay();
	void ValueChanged(const char* name, const char* val);

    WX_PG_DECLARE_BASIC_TYPE_METHODS()

protected:
	int& m_val; // reference on purpose
	wxPropertyGridPage* m_pPage;
	int m_bUserChanged;
};

#endif // INTPROPERTY_INCLUDED
