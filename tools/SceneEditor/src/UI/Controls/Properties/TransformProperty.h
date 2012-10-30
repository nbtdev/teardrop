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
		CoS::Reflection::Object* pObj,
		const CoS::Reflection::PropertyDef* pProp,
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
	CoS::Reflection::Object* m_pObj;
	const CoS::Reflection::PropertyDef* m_pProp;
	CoS::Transform m_val;

	// this is temp
	VectorProperty* m_pos;
	QuaternionProperty* m_ori;
	VectorProperty* m_scl;
	wxPropertyGridPage* m_pPage;

	bool m_bInit;
};

#endif // TRANSFORMPROPERTY_INCLUDED
