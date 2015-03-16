/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <rpc.h>
#include "UUID.h"
#include "_String.h"

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
	::UUID uuidGen;
	if (RPC_S_OK == UuidCreate(&uuidGen)) {
		*this = (UUID&)uuidGen;
	}
}

void Teardrop::UUID::toString(String& uuid) const
{
	uuid.clear();

	RPC_CSTR str;
	::UUID& uuidGen = (::UUID&)*this;
	if (RPC_S_OK == UuidToString(&uuidGen, &str)) {
		uuid = (const char*)str;
	}
}

void Teardrop::UUID::fromString(const String& str)
{
	mLow = 0;
	mHigh = 0;
	UuidFromString((RPC_CSTR)(const char*)str, (::UUID*)this);
}
