/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Mode.h"
#include "Tools/Tool.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Mode::Mode()
{
	m_pActiveView = 0;
}
//---------------------------------------------------------------------------
Mode::~Mode()
{
}
//---------------------------------------------------------------------------
void Mode::setActiveView(View* pView)
{
	m_pActiveView = pView;
	_activeViewChanged();
}
//---------------------------------------------------------------------------
void Mode::_activeViewChanged()
{
}
//---------------------------------------------------------------------------
void Mode::activate()
{
	Tool* pActiveTool = getActiveTool();

	if (pActiveTool)
		pActiveTool->activate();
}
//---------------------------------------------------------------------------
void Mode::deactivate()
{
	Tool* pActiveTool = getActiveTool();

	if (pActiveTool)
		pActiveTool->deactivate();
}
//---------------------------------------------------------------------------
ContextMenu* Mode::getContextMenuDefinition()
{
	return 0;
}
