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

#include "Config.h"
#include "Serialization.h"
#include "Util/Hash.h"
#include <assert.h>
#include <string.h>

using namespace CoS;
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
