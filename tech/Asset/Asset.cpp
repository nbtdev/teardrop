/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

int Asset::serialize(Stream& /*strm*/)
{
	return 0;
}

int Asset::deserialize(Stream& /*strm*/)
{
	return 0;
}
