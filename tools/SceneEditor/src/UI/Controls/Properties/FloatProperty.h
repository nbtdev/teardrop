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

#if !defined(FLOATPROPERTY_INCLUDED)
#define FLOATPROPERTY_INCLUDED

/*
	Wrapper/helper for the wxFloatProperty type (really
	all we are doing is adding more methods to it, basically
	to expose the IProperty interface on it)
*/

#include "wx/propgrid/props.h"
#include "Property.h"

class FloatProperty 
	:	 public wxFloatProperty
		,public IProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(FloatProperty)
public:
	FloatProperty();
    FloatProperty( 
		float& value,
		wxPropertyGridPage* pPage = 0,
		wxPGId pid = 0,
		const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
    virtual ~FloatProperty();

	void OnSetValue();

	// IProperty implementation
	void UpdateDisplay();
	void ValueChanged(const char* name, const char* val);

    WX_PG_DECLARE_BASIC_TYPE_METHODS()

protected:
	float& m_val; // reference on purpose
	wxPropertyGridPage* m_pPage;
	int m_bUserChanged;
};

#endif // FLOATPROPERTY_INCLUDED
