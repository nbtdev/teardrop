/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Variant.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
void Variant::setBool(bool b)
{
	v.b = b;
	type = VT_BOOL;
}
//---------------------------------------------------------------------------
void Variant::setFloat(float f)
{
	v.f = f;
	type = VT_FLOAT;
}
//---------------------------------------------------------------------------
void Variant::setInt(int i)
{
	v.i = i;
	type = VT_INT;
}
//---------------------------------------------------------------------------
void Variant::setString(const char* s)
{
	v.s = s;
	type = VT_STRING;
}
//---------------------------------------------------------------------------
void Variant::setUser(void* p)
{
	v.p = p;
	type = VT_USER;
}
