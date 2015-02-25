/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
