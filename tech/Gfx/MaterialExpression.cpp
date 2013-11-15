/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "MaterialExpression.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(MaterialExpression);

MaterialExpression::MaterialExpression()
{
}

MaterialExpression::~MaterialExpression()
{
}

Attribute* MaterialExpression::findInputAttribute(const char* name)
{
	for (size_t i=0; i<mInputs.size(); ++i) {
		if (!strcmp(mInputs[i].mName, name))
			return &mInputs[i];
	}

	return 0;
}

Attribute* MaterialExpression::findOutputAttribute(const char* name)
{
	for (size_t i=0; i<mOutputs.size(); ++i) {
		if (!strcmp(mOutputs[i].mName, name))
			return &mOutputs[i];
	}

	return 0;
}
