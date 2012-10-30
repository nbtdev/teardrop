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

#include "stdafx.h"
#include "Config.h"
#include "Hash.h"

using namespace CoS;
//---------------------------------------------------------------------------
size_t CoS::hashString(const char* strVal)
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
size_t CoS::hashData(void* pData, size_t len)
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
unsigned __int64 CoS::hashString64(const char* strVal)
{
	if (!strVal)
		return 0;

	const char* p = strVal;
	unsigned __int64 rtn = 0;
	while(*p)
	{
		rtn = rtn * 131 + *p;
		++p;
	}

	return rtn;
}
//---------------------------------------------------------------------------
unsigned __int64 CoS::hashData64(void* pData, size_t len)
{
	if (!pData)
		return 0;

	const char* p = (const char*)pData;
	size_t i = 0;
	unsigned __int64 rtn = 0;

	while(i < len)
	{
		rtn = rtn * 131 + *p;
		++i;
		++p;
	}

	return rtn;
}
