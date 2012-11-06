/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Attribute.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Attribute::Attribute()
{
}
//---------------------------------------------------------------------------
Attribute::Attribute(const String& value)
{
	m_value = value;
}
//---------------------------------------------------------------------------
Attribute::~Attribute()
{
}
//---------------------------------------------------------------------------
const String& Attribute::getValue() const
{
	return m_value;
}
//---------------------------------------------------------------------------
void Attribute::setValue(const String& value)
{
	m_value = value;
}
