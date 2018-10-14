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

#include "Util/UUID.h"
#include "Util/_String.h"
#include <uuid/uuid.h>

Teardrop::UUID::UUID()
	: mLow(0)
	, mHigh(0)
{
}

Teardrop::UUID::UUID(const Teardrop::UUID& other)
{
	*this = other;
}

Teardrop::UUID::~UUID()
{

}

bool Teardrop::UUID::operator==(const Teardrop::UUID& other) const
{
	return (mHigh == other.mHigh && mLow == other.mLow);
}

bool Teardrop::UUID::operator!=(const Teardrop::UUID& other) const
{
	return !(*this==other);
}

bool Teardrop::UUID::operator<(const Teardrop::UUID& other) const
{
	if (mHigh < other.mHigh) 
		return true;
	else
		if (mHigh == other.mHigh)
			return (mLow < other.mLow);

	return false;
}

void Teardrop::UUID::generate() 
{
    uuid_generate((uuid_t&)*this);
}

void Teardrop::UUID::toString(String& uuid) const
{
    uuid.clear();

    char buf[128];
    uuid_unparse((uuid_t&)*this, buf);

    uuid = buf;
}

void Teardrop::UUID::fromString(const String& str)
{
	mLow = 0;
	mHigh = 0;

    uuid_parse((const char*)str, (uuid_t&)*this);
}
