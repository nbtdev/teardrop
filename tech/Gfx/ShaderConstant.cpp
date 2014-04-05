/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ShaderConstant.h"

using namespace Teardrop;
using namespace Gfx;

ShaderConstant::ShaderConstant()
	: mType(VET_UNKNOWN)
	, mWidth(0)
	, mRank(0)
	, mVersion(0)
	, mStorage(0)
{

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