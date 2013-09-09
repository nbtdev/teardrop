/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PropertyDef.h"
#include "Util/_String.h"
#include <string.h>

using namespace Teardrop;
using namespace Reflection;
//-----------------------------------------------------------------------------
PropertyDef::PropertyDef()
{
	m_pName = 0;
	m_pTypeName = 0;
	m_pEditor = 0;
	m_pFilter = 0;
	m_pDescription = 0;
	m_pDefault = 0;
	m_pTargetType = 0;
	m_offset = 0;
	m_id = -1;
	m_pNext = 0;
	m_bPointer = 0;
	m_bCollection = 0;
	m_bBoolean = 0;
	m_bEnum = 0;
}
//-----------------------------------------------------------------------------
PropertyDef::~PropertyDef()
{
}
//-----------------------------------------------------------------------------
void PropertyDef::setDefault(const char* def)
{
	m_pDefault = def;
}
//-----------------------------------------------------------------------------
void PropertyDef::setMetaFlags()
{
	// check for collection type
	if (strstr(m_pTypeName, "CollectionPropertyType"))
	{
		m_bCollection = 1;

		// need to strip off the CollectionPropertyType<> parts
		//m_pTargetType = strchr(m_pTypeName, '<')+1;
		//char* pEnd = strchr((char*)m_pTargetType, '>');
		//if (pEnd)
		//{
		//	*pEnd = 0;
		//}
	}

	if (!strcmp("bool", m_pTypeName) || !strcmp("Boolean", m_pTypeName))
		m_bBoolean = 1;
}
