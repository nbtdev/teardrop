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

#include "PropertyDef.h"
#include "Util/_String.h"

using namespace CoS;
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
	m_bPointer = false;
	m_bCollection = false;
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
	String typeName(m_pTypeName);

	// check for pointer type
	if (typeName.contains("PointerPropertyType"))
	{
		m_bPointer = true;

		// need to strip off the PointerPropertyType<> parts
		//m_pTargetType = strchr(m_pTypeName, '<')+1;
		//char* pEnd = strchr((char*)m_pTargetType, '>');
		//if (pEnd)
		//{
		//	*pEnd = 0;
		//}
	}

	// check for collection type
	if (typeName.contains("CollectionPropertyType"))
	{
		m_bCollection = true;

		// need to strip off the CollectionPropertyType<> parts
		//m_pTargetType = strchr(m_pTypeName, '<')+1;
		//char* pEnd = strchr((char*)m_pTargetType, '>');
		//if (pEnd)
		//{
		//	*pEnd = 0;
		//}
	}
}
