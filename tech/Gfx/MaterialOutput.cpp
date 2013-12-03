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
	mInputs.push_back(Attribute("Specular", ATTR_RGB, this, Attribute::Optional, "RGB(0,0,0)"));
	mInputs.push_back(Attribute("Emissive", ATTR_RGB, this, Attribute::Optional, "RGB(0,0,0)"));
	mInputs.push_back(Attribute("Ambient", ATTR_RGB, this, Attribute::Optional, "RGB(0,0,0)"));
	mInputs.push_back(Attribute("Specular Power", ATTR_FLOAT, this, Attribute::Optional, "0"));

	mInputs.push_back(Attribute("Normal", ATTR_FLOAT3, this, Attribute::Optional, "float3(0,1,0)"));

	return true;
}

static const char* BODY =
"    c = RGBA(1);\n"
;

void MaterialOutput::appendBody(Language /*lang*/, std::ostream& o)
{
	// it's the same in all languages
	o << BODY;
}
