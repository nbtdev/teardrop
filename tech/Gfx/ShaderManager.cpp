/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ShaderManager.h"
#include "Material.h"

using namespace Teardrop;
using namespace Gfx;

ShaderManager* Singleton<ShaderManager>::mInst = 0;

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{

}

