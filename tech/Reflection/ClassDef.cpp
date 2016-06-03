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

#include "ClassDef.h"
#include "PropertyDef.h"
#include "EnumDef.h"
#include "Memory/Memory.h"
#include "Util/Hash.h"
#include <string.h>

using namespace Teardrop;
using namespace Reflection;
//---------------------------------------------------------------------------
static ClassDef* s_pRoot = 0;
static int s_nextClassId = 0;
//---------------------------------------------------------------------------
static void appendClassDef(ClassDef** pRoot, ClassDef* pClassDefToAdd)
{
    pClassDefToAdd->m_pNext = *pRoot;
    *pRoot = pClassDefToAdd;
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
ClassDef::ClassDef(const char* pName, const char* pBaseClassName)
{
	m_pName = pName;
	m_pBaseClass = 0;
	m_pBaseClassName = pBaseClassName;
	m_bCreatable = false;
	m_pNext = 0;
	m_pProps = 0;
	m_pEnums = 0;
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
const ClassDef* ClassDef::getBaseClass() const
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
//---------------------------------------------------------------------------
void ClassDef::addEnum(EnumDef* pEnum)
{
	EnumDef** ppEnum = &m_pEnums;

	while (*ppEnum) ppEnum = &((*ppEnum)->mNext);
	*ppEnum = pEnum;
}
//-----------------------------------------------------------------------------
const PropertyDef* ClassDef::findProperty(const char* pPropName, bool deep) const
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

		const ClassDef* pClass = m_pBaseClass;

		while (pClass)
		{
			const PropertyDef* pProp = pClass->getProps();
			while (pProp)
			{
				if (strcmp(pPropName, pProp->getName()) == 0)
					return pProp;

				pProp = pProp->m_pNext;
			}

			pClass = pClass->getBaseClass();
		}
	}

	return 0;
}
//-----------------------------------------------------------------------------
const EnumDef* ClassDef::findEnum(const char* pEnumName, bool deep) const
{
	// walk the list to find the enum
	EnumDef* pTmp = m_pEnums;
	while (pTmp)
	{
		if (strcmp(pEnumName, pTmp->name()) == 0)
			return pTmp;

		pTmp = pTmp->mNext;
	}

	// if deep, search the superclasses too; expensive, is why it defaults to false
	if (deep)
	{
		if (!m_pBaseClass)
			m_pBaseClass = findClassDef(m_pBaseClassName);

		const ClassDef* pClass = m_pBaseClass;

		while (pClass)
		{
			const EnumDef* pEnum = pClass->getEnums();
			while (pEnum)
			{
				if (strcmp(pEnumName, pEnum->name()) == 0)
					return pEnum;

				pEnum = pEnum->mNext;
			}

			pClass = pClass->getBaseClass();
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
	if (this == pClass)
		return true;

	const ClassDef* pTest = this;
	while (pTest && pTest != pClass)
	{
		pTest = pTest->getBaseClass();
		if (pTest == pClass)
			return true;
	}

	return false;
}
