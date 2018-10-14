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

#include "EnumDef.h"

using namespace Teardrop;
using namespace Reflection;

EnumDef::EnumDef(const char* name, const char* desc)
	: mNext(0)
	, mValues(0)
	, mName(name)
	, mDesc(desc)
	, mNumValues(0)
{
	
}

EnumDef::~EnumDef()
{

}

const char* EnumDef::name() const
{
	return mName;
}

const char* EnumDef::description() const
{
	return mDesc;
}

const EnumValue* EnumDef::values() const
{
	return mValues;
}

int EnumDef::numValues() const
{
	return mNumValues;
}

void EnumDef::addValue(EnumValue* value)
{
	EnumValue** ppValue = &mValues;
	while (*ppValue) 
		ppValue = &(*ppValue)->mNext;
	
	*ppValue = value;
	++mNumValues;
}

const EnumValue* EnumDef::findByValue(int value) const
{
	EnumValue* const* ppValue = &mValues;
	while (*ppValue) {
		if ((*ppValue)->mValue == value)
			return *ppValue;

		ppValue = &(*ppValue)->mNext;
	}

	return 0;
}

EnumValue::EnumValue(const char* id, const char* desc, int value)
	: mNext(0)
	, mId(id)
	, mDescription(desc)
	, mValue(value)
{

}

EnumValue::~EnumValue()
{

}

const char* EnumValue::id() const
{
	return mId;
}

const char* EnumValue::description() const
{
	return mDescription;
}

int EnumValue::value() const
{
	return mValue;
}
