/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "MessageBus.h"
#include "PropertyGrid.h"
#include "Properties/TransformProperty.h"
#include "Properties/VectorProperty.h"
#include "Properties/QuaternionProperty.h"
#include "Properties/IntProperty.h"
#include "Properties/FloatProperty.h"
#include "Properties/BoolProperty.h"
#include "Properties/StringProperty.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
// TODO: this is all a temp hack for now, so that we can get notification about
// property changes, in the absence of the message system in-game

static struct PropChanged { 
	PropChanged() {
		Reflection::Object::setBroadcastPropertyChangedFn(
			objectPropertyChangedCallback);
	};

	static void objectPropertyChangedCallback(
		Reflection::Object* pObj, const Reflection::PropertyDef* pProp)
	{
		if (m_delegate) m_delegate(pObj, pProp);
	}

	typedef fastdelegate::FastDelegate2<
		Reflection::Object*, const Reflection::PropertyDef*> Delegate;
	static Delegate m_delegate;
} s_PropChanged;
PropChanged::Delegate PropChanged::m_delegate;
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(PropertyGridControl, wxPropertyGridManager)
	EVT_PG_SELECTED(wxID_ANY, PropertyGridControl::onSelectedPropertyChanged)
	EVT_PG_CHANGED(wxID_ANY, PropertyGridControl::onPropertyChanged)
	EVT_PG_CHANGING(wxID_ANY, PropertyGridControl::onPropertyChanging)
END_EVENT_TABLE()
//---------------------------------------------------------------------------
PropertyGridControl::PropertyGridControl(wxWindow* pParent)
	: wxPropertyGridManager(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxPG_TOOLBAR|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER)
{
	m_pSelected = 0;
	m_pMsgBus = 0;

	AddPage("Properties");
	m_pPage = GetLastPage();
	s_PropChanged.m_delegate = fastdelegate::MakeDelegate(
		this, &PropertyGridControl::onObjectPropertyChanged);
}
//---------------------------------------------------------------------------
PropertyGridControl::~PropertyGridControl()
{
	s_PropChanged.m_delegate.clear();
}
//---------------------------------------------------------------------------
void PropertyGridControl::connect(MessageBus* pMsgBus)
{
	m_pMsgBus = pMsgBus;

	if (pMsgBus)
	{
		pMsgBus->ObjectSelectionChanged.connect(
			this, &PropertyGridControl::selectedObjectChanged);
	}
}
//---------------------------------------------------------------------------
void PropertyGridControl::disconnect()
{
	if (m_pMsgBus)
	{
		m_pMsgBus->ObjectSelectionChanged.disconnect(
			this, &PropertyGridControl::selectedObjectChanged);
	}

	m_pMsgBus = 0;
}
//---------------------------------------------------------------------------
void PropertyGridControl::selectedObjectChanged(
	void* sender, Reflection::Object* pObj)
{
	if (sender == this)
		return;

	setSelectedObject(pObj);
}
//---------------------------------------------------------------------------
void PropertyGridControl::setSelectedObject(Teardrop::Reflection::Object* pObj)
{
	if (m_pSelected == pObj)
		return;

	m_pSelected = pObj;
	resetProperties();
}
//---------------------------------------------------------------------------
Teardrop::Reflection::Object* PropertyGridControl::getSelectedObject()
{
	return m_pSelected;
}
//---------------------------------------------------------------------------
void PropertyGridControl::appendPropertyToPage(
	Reflection::Object* pObj, 
	const Reflection::PropertyDef* pProp
	)
{
	String propName(pProp->getName());
	String propType(pProp->getTypeName());

	if (propType == "int")
	{
		int* val = (int*)((char*)pObj + pProp->getOffset());

		IntProperty* prop = 
			new IntProperty(*val, m_pPage, 0, (const char*)propName);
		prop->SetClientData((wxPGProperty::ClientDataType)pProp);
		prop->SetHelpString(pProp->getDescription());
		m_pPage->Append(prop);
		m_CoSToWxLut[pProp] = prop;
	}

	if (propType == "float")
	{
		float* val = (float*)((char*)pObj + pProp->getOffset());

		FloatProperty* prop = 
			new FloatProperty(*val, m_pPage, 0, (const char*)propName);
		prop->SetClientData((wxPGProperty::ClientDataType)pProp);
		prop->SetHelpString(pProp->getDescription());
		m_pPage->Append(prop);
		m_CoSToWxLut[pProp] = prop;
	}

	if (propType == "bool")
	{
		bool* val = (bool*)((char*)pObj + pProp->getOffset());

		BoolProperty* prop = 
			new BoolProperty(*val, m_pPage, 0, (const char*)propName);
		prop->SetClientData((wxPGProperty::ClientDataType)pProp);
		prop->SetHelpString(pProp->getDescription());
		m_pPage->Append(prop);
		m_CoSToWxLut[pProp] = prop;
	}

	if (propType == "String")
	{
		String* val = (String*)((char*)pObj + pProp->getOffset());

		StringProperty* prop = 
			new StringProperty(*val, m_pPage, 0, (const char*)propName);
		prop->SetClientData((wxStringProperty::ClientDataType)pProp);
		prop->SetHelpString(pProp->getDescription());
		m_pPage->Append(prop);
		m_CoSToWxLut[pProp] = prop;
	}

	if (propType == "Transform")
	{
		Transform val;
		pProp->getData(pObj, &val);

		//void* pDataAddr = (char*)pObj + pProp->getOffset();

		TransformProperty* prop = 
			new TransformProperty(
				pObj,
				pProp,
				m_pPage,
				(const char*)propName, 
				wxPG_LABEL);

		prop->SetClientData((wxStringProperty::ClientDataType)pProp);
		prop->SetHelpString(pProp->getDescription());
		m_CoSToWxLut[pProp] = prop;
	}

	if (propType == "Vector4")
	{
		Vector4* val = (Vector4*)((char*)pObj + pProp->getOffset());

		VectorProperty* prop = 
			new VectorProperty(*val, m_pPage, 0, (const char*)propName);

		prop->SetClientData((wxStringProperty::ClientDataType)pProp);
		prop->SetHelpString(pProp->getDescription());
		m_CoSToWxLut[pProp] = prop;
	}
}
//---------------------------------------------------------------------------
void PropertyGridControl::resetProperties()
{
	// clear the propgrid first; if the selected object is null, then
	// that's the behavior we want anyway
	ClearPage(0);

	m_CoSToWxLut.clear();

	if (m_pSelected)
	{
		Teardrop::Reflection::ClassDef* pClassDef = m_pSelected->getDerivedClassDef();

		if (m_pSelected->getDerivedClassDef()->isA(ZoneObject::getClassDef()))
		{
			ZoneObject* pZoneObj = static_cast<ZoneObject*>(m_pSelected);

			// add categories for the components that the object has
			const ZoneObject::Components& comps = pZoneObj->getComponents();
			for (ZoneObject::Components::const_iterator it = comps.begin();
				it != comps.end(); ++it)
			{
				Component* pComp = it->second;
				m_pPage->Append(new wxPropertyCategory(pComp->getDerivedClassDef()->getName()));
				const Teardrop::Reflection::PropertyDef* pProp = 
					pComp->getDerivedClassDef()->getProps();

				// append component properties
				while (pProp)
				{
					appendPropertyToPage(pComp, pProp);
					pProp = pProp->m_pNext;
				}
			}
		}

		while (pClassDef)
		{
			// add a category
			m_pPage->Append(new wxPropertyCategory(pClassDef->getName()));

			// then figure out what properties we want to display
			const Teardrop::Reflection::PropertyDef* pProp = pClassDef->getProps();

			while (pProp)
			{
				appendPropertyToPage(m_pSelected, pProp);
				pProp = pProp->m_pNext;
			}

			const char* baseName = pClassDef->getBaseClassName();
			if (baseName)
			{
				pClassDef = Teardrop::Reflection::ClassDef::findClassDef(baseName);
			}
		}
	}

	Refresh();
}
//---------------------------------------------------------------------------
void PropertyGridControl::onPropertyChanged(wxPropertyGridEvent& evt)
{
	if (!m_pSelected)
		return;

	wxPGProperty* wxProp = evt.GetProperty();
	if (!wxProp)
		return;

	Reflection::PropertyDef* pProp = static_cast<Reflection::PropertyDef*>(
		wxProp->GetClientData());

	if (pProp)
	{
		String propType(pProp->getTypeName());

		if (propType == "int")
		{
			int val = evt.GetPropertyValueAsInt();
			pProp->setData(m_pSelected, &val);
		}

		if (propType == "float")
		{
			float val = (float)evt.GetPropertyValueAsDouble();
			pProp->setData(m_pSelected, &val);
		}

		if (propType == "String")
		{
			String val(evt.GetPropertyValueAsString());
			pProp->setData(m_pSelected, &val);
		}

		if (propType == "bool")
		{
			bool val = (0 != evt.GetPropertyValueAsBool());
			pProp->setData(m_pSelected, &val);
		}
	}

	PropertyChanged(m_pSelected, pProp);
}
//---------------------------------------------------------------------------
void PropertyGridControl::onObjectPropertyChanged(
	Reflection::Object* /*pObj*/, const Reflection::PropertyDef* pProp)
{
	// look up the wx PG control from the passed-in prop
	CoSToWxPropLUT::iterator it = m_CoSToWxLut.find(pProp);
	if (it != m_CoSToWxLut.end())
	{
		IProperty* prop = it->second;
		prop->ValueChanged(0, 0);
		prop->UpdateDisplay();
	}
}
//---------------------------------------------------------------------------
void PropertyGridControl::onSelectedPropertyChanged(wxPropertyGridEvent& evt)
{
	wxPGProperty* wxProp = evt.GetProperty();
	if (!wxProp)
		return;

	Reflection::PropertyDef* pProp = static_cast<Reflection::PropertyDef*>(
		wxProp->GetClientData());

	SelectedPropertyChanged(m_pSelected, 0, pProp);
}
//---------------------------------------------------------------------------
void PropertyGridControl::onPropertyChanging(wxPropertyGridEvent& /*evt*/)
{
}
