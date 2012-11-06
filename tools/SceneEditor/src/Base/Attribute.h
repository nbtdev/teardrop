/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ATTRIBUTE_INCLUDED)
#define ATTRIBUTE_INCLUDED

#include "Util/_String.h"
#include "Memory/Memory.h"

/*
	class Attribute

	Base class for an arbitrary attribute value
*/

class Attribute
{
public:
	Attribute();
	Attribute(const Teardrop::String& value);
	virtual ~Attribute();

	const Teardrop::String& getValue() const;
	void setValue(const Teardrop::String& val);

	TD_DECLARE_ALLOCATOR();

protected:
	Teardrop::String m_value;
};

#endif // ATTRIBUTE_INCLUDED
