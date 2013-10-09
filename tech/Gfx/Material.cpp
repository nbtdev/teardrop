/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Material.h"
#include "Sampler2D.h"
#include "Shader.h"
#include "ShaderManager.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(Material);

Material::Material()
	: mSamplers2D(0)
	, mNumSamplers2D(-1) // -1 indicates material not yet initialized
	, mShader(0)
{
}

Material::~Material()
{
	delete [] mSamplers2D;
}

bool Material::initialize()
{
	return true;
}

bool Material::destroy()
{
	ShaderManager::instance().release(mShader);
	mShader = 0;
	return true;
}

Sampler2D* Material::samplers2D()
{
	if (mNumSamplers2D < 0)
		initialize();

	return mSamplers2D;
}

int Material::numSamplers2D()
{
	if (mNumSamplers2D < 0)
		initialize();

	return mNumSamplers2D;
}

Shader* Material::shader()
{
	if (mNumSamplers2D < 0)
		initialize();

	return mShader;
}
