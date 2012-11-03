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
#include "SerialUtil.h"
#include "Stream/Stream.h"
#include "Util/_String.h"
#include <string>
#include <stdio.h>
#include <string.h>
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
bool Serial::Util::write(char c, Stream& s)
{
	return s.write(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(unsigned char c, Stream& s)
{
	return s.write(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(short c, Stream& s)
{
	return s.write(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(unsigned short c, Stream& s)
{
	return s.write(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(int c, Stream& s)
{
	return s.write(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(unsigned int c, Stream& s)
{
	return s.write(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(long c, Stream& s)
{
	return s.write(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(unsigned long c, Stream& s)
{
	return s.write(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(const char* c, Stream& s)
{
	return s.write((void*)c, strlen(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::write(const String& string, Stream& strm)
{
	return string.serialize(strm);
}
//---------------------------------------------------------------------------
bool Serial::Util::read(char& c, Stream& s)
{
	return s.read(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::read(unsigned char& c, Stream& s)
{
	return s.read(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::read(short& c, Stream& s)
{
	return s.read(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::read(unsigned short& c, Stream& s)
{
	return s.read(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::read(int& c, Stream& s)
{
	return s.read(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::read(unsigned int& c, Stream& s)
{
	return s.read(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::read(long& c, Stream& s)
{
	return s.read(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::read(unsigned long& c, Stream& s)
{
	return s.read(&c, sizeof(c));
}
//---------------------------------------------------------------------------
bool Serial::Util::read(char* c, Stream& s, size_t len)
{
	return s.read(c, len);
}
//---------------------------------------------------------------------------
bool Serial::Util::read(String& string, Stream& strm)
{
	return string.deserialize(strm);
}
//---------------------------------------------------------------------------
bool Serial::Util::verifyPlatformDataSizes()
{
	// assert datatype sizes
	assert(sizeof(bool) == 1);
	assert(sizeof(char) == 1);
	assert(sizeof(unsigned char) == 1);
	assert(sizeof(short) == 2);
	assert(sizeof(unsigned short) == 2);
	assert(sizeof(int) == 4);
	assert(sizeof(unsigned int) == 4);
	assert(sizeof(long) == 4);
	assert(sizeof(unsigned long) == 4);
	assert(sizeof(void*) == 4);
	assert(sizeof(size_t) == 4);

	return (
		sizeof(bool) == 1
		&& sizeof(char) == 1
		&& sizeof(unsigned char) == 1
		&& sizeof(short) == 2
		&& sizeof(unsigned short) == 2
		&& sizeof(int) == 4
		&& sizeof(unsigned int) == 4
		&& sizeof(long) == 4
		&& sizeof(unsigned long) == 4
		&& sizeof(void*) == 4
		&& sizeof(size_t) == 4
		);
}
//---------------------------------------------------------------------------
void Serial::Util::mapResourceDependencyName(const char* src, char* dest, size_t destLen)
{
	std::string in(src);

	// strip off any path information
	in = in.substr(in.find_last_of("\\/")+1);

	// get extension
	size_t pos = in.find_last_of('.');
	std::string ext(in.substr(pos+1));
	std::string name(in.substr(0, pos));

	if (ext == "TGA" || ext == "tga"
		|| ext == "PNG" || ext == "png"
		|| ext == "DDS" || ext == "dds"
		|| ext == "JPG" || ext == "JPG")
	{
		sprintf_s(dest, destLen, "textures/%s.%s", name.c_str(), "tex");
	}
	else
	{
		sprintf_s(dest, destLen, "%s", in.c_str());
	}
}
