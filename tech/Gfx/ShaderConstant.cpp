/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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

const void* ShaderConstant::data() const
{
	return mStorage;
}

int ShaderConstant::version() const
{
	return mVersion;
}

void ShaderConstant::set(const void* data)
{
	if (mStorage) {
		memcpy(mStorage, data, mLen);
	}
}