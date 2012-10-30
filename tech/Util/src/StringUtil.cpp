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
#include "StringUtil.h"
#include "_String.h"
#include <stdlib.h>
#include <stdio.h>

using namespace CoS;
//---------------------------------------------------------------------------
void StringUtil::toString(float c, String& str)
{
	char buf[64];
	toString(c, buf, 64);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(float c, char* buf, int size)
{
	sprintf_s(buf, size, "%f", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(bool c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(bool c, char* buf, int size)
{
	sprintf_s(buf, size, "%s", c ? "true" : "false");
}
//---------------------------------------------------------------------------
void StringUtil::toString(char c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(char c, char* buf, int size)
{
	sprintf_s(buf, size, "%d", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(unsigned char c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(unsigned char c, char* buf, int size)
{
	sprintf_s(buf, size, "%u", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(short c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(short c, char* buf, int size)
{
	sprintf_s(buf, size, "%d", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(unsigned short c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(unsigned short c, char* buf, int size)
{
	sprintf_s(buf, size, "%u", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(int c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(int c, char* buf, int size)
{
	sprintf_s(buf, size, "%d", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(unsigned int c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(unsigned int c, char* buf, int size)
{
	sprintf_s(buf, size, "%u", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(long c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(long c, char* buf, int size)
{
	sprintf_s(buf, size, "%d", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(unsigned long c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(unsigned long c, char* buf, int size)
{
	sprintf_s(buf, size, "%u", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(void* c, String& str)
{
	char buf[32];
	toString(c, buf, 32);
	str = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toString(void* c, char* buf, int size)
{
	sprintf_s(buf, size, "%d", c);
}
//---------------------------------------------------------------------------
void StringUtil::toString(const String& out, String& in)
{
	in = out;
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, float& c)
{
	c = (float)atof(str);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, float& c)
{
	c = (float)atof(buf);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, bool& c)
{
	c = (str == "true" ? true : false);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, bool& c)
{
	fromString(String(buf), c);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, char& c)
{
	c = (char)atoi(str);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, char& c)
{
	c = (char)atoi(buf);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, unsigned char& c)
{
	c = (unsigned char)strtoul(str, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, unsigned char& c)
{
	c = (unsigned char)strtoul(buf, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, short& c)
{
	c = (short)atoi(str);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, short& c)
{
	c = (short)atoi(buf);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, unsigned short& c)
{
	c = (unsigned short)strtoul(str, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, unsigned short& c)
{
	c = (unsigned short)strtoul(buf, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, int& c)
{
	c = atoi(str);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, int& c)
{
	c = atoi(buf);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, unsigned int& c)
{
	c = (int)strtoul(str, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, unsigned int& c)
{
	c = (int)strtoul(buf, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, long& c)
{
	c = strtol(str, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, long& c)
{
	c = strtol(buf, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, unsigned long& c)
{
	c = strtoul(str, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, unsigned long& c)
{
	c = strtoul(buf, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& str, void*& c)
{
	c = (void*)(size_t)strtoul(str, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const char* buf, void*& c)
{
	c = (void*)(size_t)strtoul(buf, 0, 10);
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& in, String& out)
{
	out = in;
}
//---------------------------------------------------------------------------
// TODO: move this out to Math lib somehow (probably have to make StringUtil
// a namespace and not a class)
#include "Math/include/Transform.h"
#include "Math/include/Vector4.h"
#include "Math/include/Vector2.h"
#include "Math/include/Quaternion.h"
void StringUtil::fromString(const String& in, Transform& out)
{
	// format of Transform string rep is [(px,py,pz),(qw,qx,qy,qz),(sx,sy,sz)]
	sscanf_s(in, 
		"[(%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f)]",
		&out.trans.x, 
		&out.trans.y, 
		&out.trans.z,
		&out.rot.w,
		&out.rot.x,
		&out.rot.y,
		&out.rot.z,
		&out.scale.x,
		&out.scale.y,
		&out.scale.z);

	out.trans.w = 0;
	out.scale.w = 0;
}
//---------------------------------------------------------------------------
void StringUtil::toString(const Transform& in, String& out)
{
	// format of Transform string rep is [(px,py,pz),(qw,qx,qy,qz),(sx,sy,sz)]
	char buf[256];
	sprintf_s(buf, 256, 
		"[(%0.6f,%0.6f,%0.6f),(%0.6f,%0.6f,%0.6f,%0.6f),(%0.6f,%0.6f,%0.6f)]",
		in.trans.x, 
		in.trans.y, 
		in.trans.z,
		in.rot.w,
		in.rot.x,
		in.rot.y,
		in.rot.z,
		in.scale.x,
		in.scale.y,
		in.scale.z);
	out = buf;
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& in, Vector4& out)
{
#if defined(WIN32)
	sscanf_s(in, "(%f,%f,%f,%f)", &out.x, &out.y, &out.z, &out.w);
#else
	sscanf(in, "(%f,%f,%f,%f)", &out.x, &out.y, &out.z, &out.w);
#endif
}
//---------------------------------------------------------------------------
void StringUtil::toString(const Vector4& in, String& out)
{
	char buf[256];
	sprintf_s(buf, 256, 
		"(%0.6f,%0.6f,%0.6f,%0.6f)",
		in.x, 
		in.y, 
		in.z,
		in.w);
	out = buf;
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& in, Vector2& out)
{
#if defined(WIN32)
	sscanf_s(in, "(%f,%f)", &out.x, &out.y);
#else
	sscanf(in, "(%f,%f)", &out.x, &out.y);
#endif
}
//---------------------------------------------------------------------------
void StringUtil::toString(const Vector2& in, String& out)
{
	char buf[256];
	sprintf_s(buf, 256, 
		"(%0.6f,%0.6f)",
		in.x, 
		in.y);
	out = buf;
}
//---------------------------------------------------------------------------
void StringUtil::fromString(const String& in, Quaternion& out)
{
#if defined(WIN32)
	sscanf_s(in, "(%f,%f,%f,%f)", &out.w, &out.x, &out.y, &out.z);
#else
	sscanf(in, "(%f,%f,%f,%f)", &out.w, &out.x, &out.y, &out.z);
#endif
}
//---------------------------------------------------------------------------
void StringUtil::toString(const Quaternion& in, String& out)
{
	char buf[256];
	sprintf_s(buf, 256, 
		"(%0.6f,%0.6f,%0.6f,%0.6f)",
		in.w,
		in.x, 
		in.y, 
		in.z);
	out = buf;
}
//---------------------------------------------------------------------------
void StringUtil::toUNIXPath(String& s)
{
	for (size_t i=0; i<s.length(); ++i)
		if (s[i] == '\\')
			s[i] = '/';
}