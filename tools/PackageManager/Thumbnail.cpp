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

#include "Thumbnail.h"
#include <algorithm>
#include <cstring>

using namespace Teardrop;
using namespace Tools;

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
