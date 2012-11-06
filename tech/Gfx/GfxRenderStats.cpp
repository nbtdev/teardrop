/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxRenderStats.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxRenderStats::GfxRenderStats()
{
	reset();
}
//---------------------------------------------------------------------------
GfxRenderStats::GfxRenderStats(const GfxRenderStats& other)
{
	*this = other;
}
//---------------------------------------------------------------------------
GfxRenderStats::~GfxRenderStats()
{
}
//---------------------------------------------------------------------------
GfxRenderStats& GfxRenderStats::operator=(const GfxRenderStats& other)
{
	frameStart = other.frameStart;
	frameEnd = other.frameEnd;
	numBatches = other.numBatches;
	numTris = other.numTris;
	numVerts = other.numVerts;
	return *this;
}
//---------------------------------------------------------------------------
void GfxRenderStats::reset()
{
	frameStart = 0;
	frameEnd = 0;
	numBatches = 0;
	numTris = 0;
	numVerts = 0;
}
