/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(STRINGPROPERTY_INCLUDED)
#define STRINGPROPERTY_INCLUDED

/*
	Wrapper/helper for the wxStringProperty type (really
	all we are doing is adding more methods to it, basically
	to expose the IProperty interface on it)
*/

#include "wx/propgrid/props.h"
#include "Property.h"

namespace Teardrop
{
	class String;
}

class StringProperty 
	:	 public wxStringProperty
		,public IProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(StringProperty)
public:
	StringProperty();
    StringProperty( 
		Teardrop::String& value,
		wxPropertyGridPage* pPage = 0,
		wxPGId pid = 0,
		const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
    virtual ~StringProperty();

	void OnSetValue();

	// IProperty implementation
	void UpdateDisplay();
	void ValueChanged(const char* name, const char* val);

    WX_PG_DECLARE_BASIC_TYPE_METHODS()

protected:
	Teardrop::String& m_val; // reference on purpose
	wxPropertyGridPage* m_pPage;
	int m_bUserChanged;
};

#endif // STRINGPROPERTY_INCLUDED
