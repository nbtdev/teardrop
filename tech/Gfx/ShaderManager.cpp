/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "ShaderManager.h"
#include "Material.h"
#include "ShaderConstantTable.h"

using namespace Teardrop;
using namespace Gfx;

template<> ShaderManager* Singleton<ShaderManager>::mInst = 0;

ShaderManager::ShaderManager()
	: mConstants(TD_NEW ShaderConstantTable)
{
	assert(!mInst);
	mInst = this;
}

ShaderManager::~ShaderManager()
{
	delete mConstants;
}

ShaderConstantTable* ShaderManager::constantTable()
{
	return mConstants;
}
