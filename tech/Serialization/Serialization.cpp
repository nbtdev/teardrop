/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "Serialization.h"
#include "Util/Hash.h"
#include <assert.h>
#include <string.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
static SerialClass* s_pRoot = 0;
//---------------------------------------------------------------------------
static void addClass(SerialClass** pRoot, SerialClass* pClassToAdd)
{
	if (!*pRoot)
	{
		*pRoot = pClassToAdd;
		return;
	}

	// else walk the list to find the end, and append there
	SerialClass* pTmp = *pRoot;
	while (pTmp->pNext)
	{
		pTmp = pTmp->pNext;
	}

	pTmp->pNext = pClassToAdd;
}
//---------------------------------------------------------------------------
SerialClass::SerialClass(const char* name, bool hasVtabl)
{
	this->name = name;
	addr = this;
	id = hashString64(name);
	bHasVtabl = hasVtabl;
	pNext = 0;
	addClass(&s_pRoot, this);
}
//---------------------------------------------------------------------------
const char* SerialClass::getName() const
{
	return name;
}
//---------------------------------------------------------------------------
const void* SerialClass::getAddr() const
{
	return (void*)*((size_t*)addr);
}
//---------------------------------------------------------------------------
void SerialClass::setAddr(void* pClassInstAddr)
{
	addr = pClassInstAddr;
}
//---------------------------------------------------------------------------
unsigned __int64 SerialClass::getId() const
{
	return id;
}
//---------------------------------------------------------------------------
bool SerialClass::hasVtabl() const
{
	return bHasVtabl;
}
//---------------------------------------------------------------------------
SerialClass* SerialClass::findClassById(unsigned __int64 id)
{
	if (s_pRoot)
	{
		SerialClass* pTmp = s_pRoot;
		while (pTmp)
		{
			if (id == pTmp->getId())
			{
				return pTmp;
			}

			pTmp = pTmp->pNext;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
SerialClass* SerialClass::findClassByName(const char* name)
{
	if (s_pRoot)
	{
		SerialClass* pTmp = s_pRoot;
		while (pTmp)
		{
			if (strcmp(name, pTmp->getName()) == 0)
			{
				return pTmp;
			}

			pTmp = pTmp->pNext;
		}
	}

	return 0;
}
