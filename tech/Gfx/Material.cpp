/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "Material.h"
#include "MaterialOutput.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Connection.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(Material);

Material::Material()
	: mShader(0)
{
}

Material::~Material()
{
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

Shader* Material::shader()
{
	if (!mShader) {
		mShader = ShaderManager::instance().createOrFindInstanceOf(this);
		mShader->initialize();
	}

	return mShader;
}

void Material::addConnection(Connection* conn)
{
	mConnections.insert(Connections::value_type(conn->input(), conn));
}

void Material::apply()
{
	shader()->apply();
}