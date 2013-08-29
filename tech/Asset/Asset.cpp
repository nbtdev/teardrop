/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Asset.h"

using namespace Teardrop;

TD_CLASS_IMPL(Asset);

Asset::Asset()
	: mData(0)
	, mLength(0)
	, mDynamicData(0)
{
}

Asset::~Asset()
{
	delete [] mDynamicData;
}

const void* Asset::data()
{
	return mData;
}

int Asset::length()
{
	return mLength;
}

void Asset::setData(void* data, int len)
{
	delete [] mDynamicData;
	mDynamicData = 0;

	mData = data;
	mLength = len;
}

void* Asset::createData(int len)
{
	delete [] mDynamicData;
	mDynamicData = new unsigned char[len];
	mData = mDynamicData;
	mLength = len;
	return mData;
}
