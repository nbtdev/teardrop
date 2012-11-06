/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GRIDPROPERTY_INCLUDED)
#define GRIDPROPERTY_INCLUDED

/*
	abstract interface that PropertyGridControl properties
	need to implement
*/

class IProperty
{
public:
	virtual void UpdateDisplay() = 0;
	virtual void ValueChanged(const char* name, const char* val) = 0;
};

#endif // GRIDPROPERTY_INCLUDED
