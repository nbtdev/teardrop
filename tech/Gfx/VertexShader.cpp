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
	: mHash(0)
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

unsigned int VertexShader::hash()
{
	return mHash;
}
