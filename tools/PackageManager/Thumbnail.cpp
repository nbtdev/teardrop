/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Thumbnail.h"

using namespace Teardrop;
using namespace Tools;

Thumbnail::Thumbnail()
	: mData(0)
	, mLen(0)
	, mWidth(128)
	, mHeight(128)
{
	mLen = mWidth*mHeight*4; // enough for uncompressed RGBA8888 data
	mData = TD_NEW char[mLen];
}

Thumbnail::~Thumbnail()
{
	delete [] mData;
}

void* Thumbnail::data()
{
	return mData;
}

const void* Thumbnail::data() const
{
	return mData;
}

int Thumbnail::length() const
{
	return mLen;
}

int Thumbnail::width() const
{
	return mWidth;
}

int Thumbnail::height() const
{
	return mHeight;
}
