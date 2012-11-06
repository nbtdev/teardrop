/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "AttributeContainer.h"
#include "Attribute.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
AttributeContainer::AttributeContainer()
{
}
//---------------------------------------------------------------------------
AttributeContainer::~AttributeContainer()
{
	for (Attributes::iterator it = m_attributes.begin();
		it != m_attributes.end(); ++it)
	{
		delete it->second;
	}
}
//---------------------------------------------------------------------------
Attribute* AttributeContainer::addAttribute(const Teardrop::String& name)
{
	Attributes::iterator it = m_attributes.find(name);
	if (it != m_attributes.end())
	{
		return it->second;
	}

	Attribute* pAttr = TD_NEW Attribute;
	m_attributes[name] = pAttr;

	return pAttr;
}
//---------------------------------------------------------------------------
void AttributeContainer::removeAttribute(const Teardrop::String& name)
{
	Attributes::iterator it = m_attributes.find(name);
	if (it != m_attributes.end())
	{
		m_attributes.erase(it);
	}
}
//---------------------------------------------------------------------------
Attribute* AttributeContainer::getAttribute(const Teardrop::String& name)
{
	Attributes::iterator it = m_attributes.find(name);
	if (it != m_attributes.end())
	{
		return it->second;
	}

	return 0;
}
//---------------------------------------------------------------------------
const Attribute* AttributeContainer::getAttribute(const Teardrop::String& name) const
{
	Attributes::const_iterator it = m_attributes.find(name);
	if (it != m_attributes.end())
	{
		return it->second;
	}

	return 0;
}
