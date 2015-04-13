/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
	m_bNested = 0;
	m_bHidden = 0;
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

	if (m_pEditor && strstr("Hidden", m_pEditor))
		m_bHidden = 1;
}
