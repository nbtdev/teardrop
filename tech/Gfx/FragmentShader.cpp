/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "FragmentShader.h"
#include "Material.h"
#include "Util/UUID.h"
#include <assert.h>

using namespace Teardrop;
using namespace Gfx;

FragmentShader::FragmentShader(Material* mtl)
	: mMaterial(mtl)
{
	assert(mtl != 0);
}

FragmentShader::~FragmentShader()
{
}

bool FragmentShader::initialize()
{
	return true;
}

bool FragmentShader::destroy()
{
	return true;
}

Teardrop::UUID FragmentShader::materialId()
{
	return mMaterial->getObjectId();
}
