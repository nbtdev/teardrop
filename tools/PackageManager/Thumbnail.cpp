/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Thumbnail.h"

using namespace Teardrop;
using namespace Tools;
#include <algorithm>

Thumbnail::Thumbnail()
	: mData(0)
	, mLen(0)
	, mWidth(0)
	, mHeight(0)
{
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

bool Thumbnail::isValid() const
{
	return (mData != 0 && mLen > 0);
}

void Thumbnail::resize(int w, int h, int nBytes)
{
	mWidth = w;
	mHeight = h;
	delete [] mData;
	mData = new char[nBytes];
	mLen = nBytes;
}

int Thumbnail::setData(void* data, int nBytes)
{
	int sz = std::min(nBytes, mLen);
	if (data && sz) {
		memcpy(mData, data, sz);
	}

	return sz;
}