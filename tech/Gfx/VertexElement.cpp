/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VertexElement.h"

using namespace Teardrop;
using namespace Gfx;

static int sTypeSizes[] = {
	0,	//VET_UNKNOWN,
	1,	//VET_BYTE,
	2,	//VET_SHORT,
	4,	//VET_INT,
	2,	//VET_HALF,
	4,	//VET_FLOAT,
};

VertexElement::VertexElement()
	: mType(VET_UNKNOWN)
	, mCount(0)
	, mUsage(VEU_UNKNOWN)
	, mIndex(0)
{
}

int VertexElement::size()
{
	return sTypeSizes[mType] * mCount;
}