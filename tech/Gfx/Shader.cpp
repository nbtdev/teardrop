/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "Shader.h"
#include "Material.h"
#include "Util/UUID.h"
#include <assert.h>

using namespace Teardrop;
using namespace Gfx;

Shader::Shader(Material* mtl)
	: mMaterial(mtl)
{
	assert(mtl != 0);
}

Shader::~Shader()
{
}

bool Shader::initialize()
{
	return true;
}

bool Shader::destroy()
{
	return true;
}

Teardrop::UUID Shader::materialId()
{
	return mMaterial->getObjectId();
}
