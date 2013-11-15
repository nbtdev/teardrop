/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "MaterialOutput.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(MaterialOutput);

MaterialOutput::MaterialOutput()
{
}

MaterialOutput::~MaterialOutput()
{
}

bool MaterialOutput::initialize()
{
	mInputs.push_back(Attribute("Diffuse", ATTR_RGBA, this));
	mInputs.push_back(Attribute("Specular", ATTR_RGB, this));
	mInputs.push_back(Attribute("Emissive", ATTR_RGB, this));
	mInputs.push_back(Attribute("Ambient", ATTR_RGB, this));
	mInputs.push_back(Attribute("Specular Power", ATTR_FLOAT, this));

	mInputs.push_back(Attribute("Normal", ATTR_RGB, this));

	return true;
}
