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

#include "ClassDef.h"
#include "PropertyDef.h"
#include "Memory/include/Memory.h"
#include "Util/include/Hash.h"
#include <string.h>

using namespace CoS;
using namespace Reflection;
//---------------------------------------------------------------------------
static ClassDef* s_pRoot = 0;
static int s_nextClassId = 0;
//---------------------------------------------------------------------------
static void appendClassDef(ClassDef** pRoot, ClassDef* pClassDefToAdd)
{
	if (!*pRoot)
	{
		*pRoot = pClassDefToAdd;
		return;
	}

	// else walk the list to find the end, and append there
	ClassDef* pTmp = *pRoot;
	while (pTmp->m_pNext)
	{
		pTmp = pTmp->m_pNext;
	}

	pTmp->m_pNext = pClassDefToAdd;
}
//---------------------------------------------------------------------------
ClassDef* ClassDef::getClasses()
{
	return s_pRoot;
}
//---------------------------------------------------------------------------
ClassDef* ClassDef::findClassDef(const char* pClassName)
{
	if (!pClassName)
		return 0;

	if (s_pRoot)
	{
		ClassDef* pTmp = s_pRoot;
		while (pTmp)
		{
			if (strcmp(pClassName, pTmp->getName()) == 0)
			{
				return pTmp;
			}

			pTmp = pTmp->m_pNext;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
//ClassDef::ClassDef(const char* pName, ClassDef* pBaseClassDef)
ClassDef::ClassDef(const char* pName, const char* pBaseClassName)
{
	m_pName = pName;
	m_pBaseClass = 0;
	m_pBaseClassName = pBaseClassName;
	m_bCreatable = false;
	m_pNext = 0;
	m_pProps = 0;
	m_numProps = 0;
	m_classId = hashString64(pName);
	appendClassDef(&s_pRoot, this);
}
//---------------------------------------------------------------------------
ClassDef::~ClassDef()
{
}
//---------------------------------------------------------------------------
void ClassDef::setCreatable()
{
	m_bCreatable = true;
}
//---------------------------------------------------------------------------
ClassDef* ClassDef::getBaseClass()
{
	if (!m_pBaseClass)
		m_pBaseClass = findClassDef(m_pBaseClassName);

	return m_pBaseClass;
}
//---------------------------------------------------------------------------
void ClassDef::addProperty(PropertyDef* pProp)
{
	if (!m_pProps)
	{
		m_pProps = pProp;
		return;
	}

	// else walk the list to find the end, and append there
	PropertyDef* pTmp = m_pProps;
	while (pTmp->m_pNext)
	{
		pTmp = pTmp->m_pNext;
	}

	pTmp->m_pNext = pProp;
	pProp->setId(m_numProps++);
}
//-----------------------------------------------------------------------------
const PropertyDef* ClassDef::findProperty(const char* pPropName, bool deep)
{
	// walk the list to find the prop
	PropertyDef* pTmp = m_pProps;
	while (pTmp)
	{
		if (strcmp(pPropName, pTmp->getName()) == 0)
			return pTmp;

		pTmp = pTmp->m_pNext;
	}

	// if deep, search the superclasses too; expensive, is why it defaults to false
	if (deep)
	{
		if (!m_pBaseClass)
			m_pBaseClass = findClassDef(m_pBaseClassName);

		ClassDef* pClass = m_pBaseClass;

		while (pClass)
		{
			const PropertyDef* pProp = pClass->getProps();
			while (pProp)
			{
				if (strcmp(pPropName, pProp->getName()) == 0)
					return pProp;

				pProp = pProp->m_pNext;
			}
			pClass = pClass->m_pNext;
			if (!pClass)
				pClass = pClass->m_pNext = findClassDef(pClass->getBaseClassName());
		}
	}

	return 0;
}
//-----------------------------------------------------------------------------
const PropertyDef* ClassDef::findPropertyById(int propId) const
{
	// walk the list to find the prop
	PropertyDef* pTmp = m_pProps;
	int idx = 0;
	while (pTmp)
	{
		if (idx == propId)
			return pTmp;

		pTmp = pTmp->m_pNext;
		++idx;
	}

	return 0;
}
//-----------------------------------------------------------------------------
bool ClassDef::isA(const ClassDef* pClass) const
{
	const ClassDef* pTest = this;
	while (pTest && pTest != pClass)
	{
		pTest = findClassDef(pTest->getBaseClassName());
		if (pTest == pClass)
			return true;
	}

	return false;
}
