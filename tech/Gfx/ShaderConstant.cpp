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

#include "ShaderConstant.h"
#include <string.h>

using namespace Teardrop;
using namespace Gfx;

ShaderConstant::ShaderConstant(VertexElementType type, int width, int rows)
	: mType(type)
	, mWidth(width)
	, mRows(rows)
	, mVersion(0)
	, mStorage(0)
	, mLen(0)
{
	switch (type) {
		case VET_FLOAT:
		default:
			mLen = sizeof(float) * width * rows;
			break;
	}

	if (mLen) {
		mStorage = TD_NEW unsigned char[mLen];
		memset(mStorage, 0, mLen);
	}
}

ShaderConstant::~ShaderConstant()
{
	delete [] mStorage;
}

int ShaderConstant::width() const
{
	return mWidth;
}

int ShaderConstant::rows() const
{
	return mRows;
}

int ShaderConstant::version() const
{
	return mVersion;
}

const void* ShaderConstant::data() const
{
	return mStorage;
}

void ShaderConstant::set(const void* data)
{
	if (mStorage) {
		memcpy(mStorage, data, mLen);
	}
}
