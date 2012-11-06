/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ATTRIBUTECONTAINER_INCLUDED)
#define ATTRIBUTECONTAINER_INCLUDED

#include "Util/_String.h"
#include <map>

/*
	class AttributeContainer

	Base class for any object that can have attributes (name-value pairs) 
*/

class Attribute;

class AttributeContainer
{
public:
	AttributeContainer();
	virtual ~AttributeContainer();

	Attribute* addAttribute(const Teardrop::String& name);
	void removeAttribute(const Teardrop::String& name);
	Attribute* getAttribute(const Teardrop::String& name);
	const Attribute* getAttribute(const Teardrop::String& name) const;

protected:
	typedef std::map<Teardrop::String, Attribute*> Attributes;
	Attributes m_attributes;
};

#endif // ATTRIBUTECONTAINER_INCLUDED
