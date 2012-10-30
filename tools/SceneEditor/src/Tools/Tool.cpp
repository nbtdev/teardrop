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

#include "stdafx.h"
#include "CoreInterface.h"
#include "MessageBus.h"
#include "Tool.h"

using namespace CoS;
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
void Tool::selectedObjectChanged(void* /*sender*/, CoS::Reflection::Object* pObj)
{
	m_selectedObjects.clear();

	if (pObj)
		m_selectedObjects.push_back(pObj);
}
