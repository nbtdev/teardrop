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

#if !defined(PROPERTYGRID_INCLUDED)
#define PROPERTYGRID_INCLUDED

#include "FastDelegate.h"

namespace CoS
{
	namespace Reflection
	{
		class PropertyDef;
		class Object;
	}
}

class IProperty;
class MessageBus;

/*
	We attempt to impose the same sort of functionality on
	the wxPropGrid control as we get with the Windows.Forms
	one -- namely, the ability to set a single "object" on 
	the control and have the ability to view and edit that 
	object instance's properties. We can do this by leveraging
	the features of the CoS::Reflection namespace classes.
*/

class PropertyGridControl : public wxPropertyGridManager
{
	DECLARE_EVENT_TABLE()
	CoS::Reflection::Object* m_pSelected;

	typedef std::map<const CoS::Reflection::PropertyDef*, IProperty*> CoSToWxPropLUT;
	CoSToWxPropLUT m_CoSToWxLut;

	wxPropertyGridPage* m_pPage;
	MessageBus* m_pMsgBus;

public:
	PropertyGridControl(wxWindow* pParent);
	~PropertyGridControl();

	/*
		Set the object whose properties you wish to view/edit
	*/
	void setSelectedObject(CoS::Reflection::Object* pObj);

	/*
		Get the object whose properties currently are on display
	*/
	CoS::Reflection::Object* getSelectedObject();

	/*
		Called when a property changes in the object (so we can update
		the display)
	*/
	void onObjectPropertyChanged(
		CoS::Reflection::Object* pObj, 
		const CoS::Reflection::PropertyDef* pPropDef);

	/*
		Called by the framework during initialization
	*/
	void connect(MessageBus* pMsgBus);
	void disconnect();

	/*********************************************************
	* Events
	*********************************************************/

	/*
		Called when the user changes which property is "selected"
	*/
	func::delegate3<
		CoS::Reflection::Object*,			/* object whose property changed */
		CoS::Reflection::PropertyDef*,		/* old value of the property */
		CoS::Reflection::PropertyDef*		/* new value of the property */
	> SelectedPropertyChanged;

	/*
		Called when the user begins to change a property value; handler
		can return "true" to accept edit, or "false" to veto it
	*/
	func::delegate2<
		CoS::Reflection::Object*,			/* object whose property is changing */
		CoS::Reflection::PropertyDef*,		/* property being changed */
		bool /*return type*/
	> BeginPropertyEdit;

	/*
		Called after the user finishes changing a property value; handler
		can return "true" to accept edit, or "false" to veto it
	*/
	func::delegate2<
		CoS::Reflection::Object*,			/* object whose property is changing */
		CoS::Reflection::PropertyDef*,		/* property being changed */
		bool /*return type*/
	> EndPropertyEdit;

	/*
		Called after the user finishes editing a property, and the EndPropertyEdit
		callback has ok'ed the edit.
	*/
	func::delegate2<
		CoS::Reflection::Object*,			/* object whose property changed */
		CoS::Reflection::PropertyDef*		/* property that changed */
	> PropertyChanged;

private:
	void resetProperties();
	void selectedObjectChanged(void* sender, CoS::Reflection::Object* pObj);

	void onSelectedPropertyChanged(wxPropertyGridEvent& evt);
	void onPropertyChanged(wxPropertyGridEvent& evt);
	void onPropertyChanging(wxPropertyGridEvent& evt);
	void appendPropertyToPage(
		CoS::Reflection::Object* pObj, 
		const CoS::Reflection::PropertyDef* pProp
		);
};

#endif // PROPERTYGRID_INCLUDED
