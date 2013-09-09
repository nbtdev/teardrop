/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
