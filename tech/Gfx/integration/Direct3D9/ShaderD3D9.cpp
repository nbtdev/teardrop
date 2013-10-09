/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "integration/Direct3D9/ShaderD3D9.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

Shader::Shader(Material* mtl)
	: Gfx::Shader(mtl)
	, mFX(0)
	, mTechnique(0)
	, mTechniqueShadowCast(0)
	, mTechniqueShadowRecv(0)
{
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

} // Direct3D9
} // Gfx
} // Teardrop
