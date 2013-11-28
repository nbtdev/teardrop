/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "AddColorExpression.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(AddColorExpression);

AddColorExpression::AddColorExpression()
{
}

AddColorExpression::~AddColorExpression()
{
}

bool AddColorExpression::initialize()
{
	mInputs.push_back(Attribute("A", ATTR_RGBA, this));
	mInputs.push_back(Attribute("B", ATTR_RGBA, this));

	mOutputs.push_back(Attribute("Output", ATTR_RGBA, this));

	return true;
}

static const char* BODY =
"    c = a + b;\n"
;

void AddColorExpression::appendBody(Language /*lang*/, std::ostream& o)
{
	// it's the same in all languages
	o << BODY;
}
