/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROPERTYGRID_INCLUDED)
#define PROPERTYGRID_INCLUDED

#include "FastDelegate.h"

namespace Teardrop
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
	the features of the Teardrop::Reflection namespace classes.
*/

class PropertyGridControl : public wxPropertyGridManager
{
	DECLARE_EVENT_TABLE()
	Teardrop::Reflection::Object* m_pSelected;

	typedef std::map<const Teardrop::Reflection::PropertyDef*, IProperty*> CoSToWxPropLUT;
	CoSToWxPropLUT m_CoSToWxLut;

	wxPropertyGridPage* m_pPage;
	MessageBus* m_pMsgBus;

public:
	PropertyGridControl(wxWindow* pParent);
	~PropertyGridControl();

	/*
		Set the object whose properties you wish to view/edit
	*/
	void setSelectedObject(Teardrop::Reflection::Object* pObj);

	/*
		Get the object whose properties currently are on display
	*/
	Teardrop::Reflection::Object* getSelectedObject();

	/*
		Called when a property changes in the object (so we can update
		the display)
	*/
	void onObjectPropertyChanged(
		Teardrop::Reflection::Object* pObj, 
		const Teardrop::Reflection::PropertyDef* pPropDef);

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
		Teardrop::Reflection::Object*,			/* object whose property changed */
		Teardrop::Reflection::PropertyDef*,		/* old value of the property */
		Teardrop::Reflection::PropertyDef*		/* new value of the property */
	> SelectedPropertyChanged;

	/*
		Called when the user begins to change a property value; handler
		can return "true" to accept edit, or "false" to veto it
	*/
	func::delegate2<
		Teardrop::Reflection::Object*,			/* object whose property is changing */
		Teardrop::Reflection::PropertyDef*,		/* property being changed */
		bool /*return type*/
	> BeginPropertyEdit;

	/*
		Called after the user finishes changing a property value; handler
		can return "true" to accept edit, or "false" to veto it
	*/
	func::delegate2<
		Teardrop::Reflection::Object*,			/* object whose property is changing */
		Teardrop::Reflection::PropertyDef*,		/* property being changed */
		bool /*return type*/
	> EndPropertyEdit;

	/*
		Called after the user finishes editing a property, and the EndPropertyEdit
		callback has ok'ed the edit.
	*/
	func::delegate2<
		Teardrop::Reflection::Object*,			/* object whose property changed */
		Teardrop::Reflection::PropertyDef*		/* property that changed */
	> PropertyChanged;

private:
	void resetProperties();
	void selectedObjectChanged(void* sender, Teardrop::Reflection::Object* pObj);

	void onSelectedPropertyChanged(wxPropertyGridEvent& evt);
	void onPropertyChanged(wxPropertyGridEvent& evt);
	void onPropertyChanging(wxPropertyGridEvent& evt);
	void appendPropertyToPage(
		Teardrop::Reflection::Object* pObj, 
		const Teardrop::Reflection::PropertyDef* pProp
		);
};

#endif // PROPERTYGRID_INCLUDED
