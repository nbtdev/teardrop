/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "InputFilter.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
InputFilter::InputFilter()
{
	m_bEnabled = false;
}
//---------------------------------------------------------------------------
InputFilter::~InputFilter()
{
}
//---------------------------------------------------------------------------
bool InputFilter::isEnabled()
{
	return m_bEnabled;
}
//---------------------------------------------------------------------------
void InputFilter::enable()
{
	m_bEnabled = true;
}
//---------------------------------------------------------------------------
void InputFilter::disable()
{
	m_bEnabled = false;
}
