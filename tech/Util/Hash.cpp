/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "Hash.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
size_t Teardrop::hashString(const char* strVal)
{
	if (!strVal)
		return 0;

	const char* p = strVal;
	size_t rtn = 0;
	while(*p)
	{
		rtn = rtn * 131 + *p;
		++p;
	}

	return rtn;
}
//---------------------------------------------------------------------------
size_t Teardrop::hashData(void* pData, size_t len)
{
	if (!pData)
		return 0;

	const char* p = (const char*)pData;
	size_t i = 0;
	size_t rtn = 0;

	while(i < len)
	{
		rtn = rtn * 131 + *p;
		++i;
		++p;
	}

	return rtn;
}
//---------------------------------------------------------------------------
uint64_t Teardrop::hashString64(const char* strVal)
{
	if (!strVal)
		return 0;

	const char* p = strVal;
    uint64_t rtn = 0;
	while(*p)
	{
		rtn = rtn * 131 + *p;
		++p;
	}

	return rtn;
}
//---------------------------------------------------------------------------
uint64_t Teardrop::hashData64(void* pData, size_t len)
{
	if (!pData)
		return 0;

	const char* p = (const char*)pData;
	size_t i = 0;
    uint64_t rtn = 0;

	while(i < len)
	{
		rtn = rtn * 131 + *p;
		++i;
		++p;
	}

	return rtn;
}
