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

#include "Config.h"
#include "Stream.h"
#include "Util/StringUtil.h"
#include "Util/_String.h"
#include <string.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
const char* Stream::ENDL = "\r\n";
const char* Stream::TAB = "\t";
//---------------------------------------------------------------------------
Stream::~Stream()
{
}
//---------------------------------------------------------------------------
Stream& Stream::operator<<(const char* v)
{
	write((void*)v, strlen(v));
	return *this;
}
//---------------------------------------------------------------------------
Stream& Stream::operator<<(int v)
{
	// hack -- for now, treat it all like text
	String s;
	StringUtil::toString(v, s);
	write((void*)(const char*)s, s.length());
	return *this;
}
//---------------------------------------------------------------------------
Stream& Stream::operator<<(unsigned int v)
{
	// hack -- for now, treat it all like text
	String s;
	StringUtil::toString(v, s);
	write((void*)(const char*)s, s.length());
	return *this;
}
//---------------------------------------------------------------------------
Stream& Stream::operator<<(bool v)
{
	// convert bool to "true" or "false"
	const char* b = v ? "1" : "0";
	write((void*)b, strlen(b));
	return *this;
}
