/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
