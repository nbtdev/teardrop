/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "VertexShader.h"
#include "Submesh.h"
#include "VertexDeclaration.h"
#include "VertexElement.h"

using namespace Teardrop;
using namespace Gfx;

VertexShader::VertexShader()
{
}

VertexShader::~VertexShader()
{
}

bool VertexShader::initialize(Submesh* /*submesh*/)
{
	return true;
}

bool VertexShader::destroy()
{
	return true;
}

const ShaderFeatures& VertexShader::features()
{
	return mFeatures;
}
