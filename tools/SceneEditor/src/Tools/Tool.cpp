/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "CoreInterface.h"
#include "MessageBus.h"
#include "Tool.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Tool::Tool()
{
	m_pView = 0;
}
//---------------------------------------------------------------------------
Tool::~Tool()
{
	deactivate();
}
//---------------------------------------------------------------------------
void Tool::setActiveView(View* pView)
{
	m_pView = pView;
}
//---------------------------------------------------------------------------
void Tool::activate()
{
	CoreInterface& iface = CoreInterface::get();
	iface.pMsgBus->ObjectSelectionChanged.connect(
		this, &Tool::selectedObjectChanged);
}
//---------------------------------------------------------------------------
void Tool::deactivate()
{
	CoreInterface& iface = CoreInterface::get();
	iface.pMsgBus->ObjectSelectionChanged.disconnect(
		this, &Tool::selectedObjectChanged);
}
//---------------------------------------------------------------------------
void Tool::_onViewChanged()
{
}
//---------------------------------------------------------------------------
void Tool::setSelectedObject(Reflection::Object* pObj)
{
	selectedObjectChanged(this, pObj);
}
//---------------------------------------------------------------------------
void Tool::selectedObjectChanged(void* /*sender*/, Teardrop::Reflection::Object* pObj)
{
	m_selectedObjects.clear();

	if (pObj)
		m_selectedObjects.push_back(pObj);
}
