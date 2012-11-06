/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
