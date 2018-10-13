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

#include "Config.h"
#include "MemoryStream.h"
#include <string.h>
#include <assert.h>
using namespace Teardrop;

MemoryStream::MemoryStream()
	: mData(0)
	, mLen(0)
	, mPos(0)
	, mCapacity(1024)
	, mOwnData(true)
{
	mData = new unsigned char[mCapacity];
}

MemoryStream::MemoryStream(size_t len)
	: mData(0)
    , mLen((int)len)
	, mPos(0)
    , mCapacity((int)len)
	, mOwnData(true)
{
	mData = new unsigned char[len];
}

MemoryStream::MemoryStream(void* data, size_t len)
	: mData((unsigned char*)data)
    , mLen((int)len)
	, mPos(0)
    , mCapacity((int)len)
	, mOwnData(false)
{
}

MemoryStream::~MemoryStream()
{
	delete [] mData;
}

void MemoryStream::expand()
{
	int newCap = mCapacity * 2;
	unsigned char* newData = new unsigned char[newCap];
	memcpy(newData, mData, mLen);
	delete [] mData;
	mOwnData = true;
	mData = newData;
	mCapacity = newCap;
}

int MemoryStream::read(void* data, size_t len, bool /*async*/)
{
	int l = int(len);
	assert(mPos+l < mLen-1);
	if (mPos+l < mLen-1) {
		memcpy(data, mData+mPos, len);
		mPos += l;
        return (int)len;
	}

	return 0;
}

int MemoryStream::write(const void* data, size_t len, bool /*async*/)
{
	int l = int(len);
	while ((l+mLen) > mCapacity) 
		expand();

	unsigned char* p = mData;
	memcpy(p+mPos, data, len);
	mPos += l;
	mLen += l;

    return (int)len;
}

size_t MemoryStream::length()
{
	return mLen;
}

size_t MemoryStream::getPosition()
{
	return mPos;
}

bool MemoryStream::seek(int offset, SeekType seekType, bool /*async*/)
{
	switch (seekType)
	{
	case BEGIN:
		assert(offset >= 0);
		assert(offset < mLen);
		if (offset >= 0 && offset < mLen) mPos = offset;
		else return false;
		break;
	case CURRENT:
		assert(offset+mPos >= 0 && offset+mPos < mLen);
		if (offset+mPos >= 0 && offset+mPos < mLen)
			mPos += offset;
		else
			return false;
		break;
	case END:
		assert(offset >= 0);
		assert(offset < mLen);
		if (offset >= 0 && offset < mLen) 
			mPos = mLen - offset - 1;
		else 
			return false;
		break;
	}

	return true;
}

bool MemoryStream::isEnd()
{
	// if we are at the end, then the byte index will be 
	// one past the end of the valid data range
	return mPos == mLen;
}

void* MemoryStream::data()
{
	return mData;
}
