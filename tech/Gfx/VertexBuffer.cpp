/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "VertexBuffer.h"
#include <assert.h>

using namespace Teardrop;
using namespace Gfx;

VertexBuffer::VertexBuffer(Submesh* parent)
	: mSize(0)
	, mCount(0)
	, mParent(parent)
{
}

VertexBuffer::~VertexBuffer()
{
}

int VertexBuffer::vertexCount()
{
	return mCount;
}

int VertexBuffer::vertexSize()
{
	return mSize;
}

void VertexBuffer::beginAddVertexElements()
{	
}

int VertexBuffer::endAddVertexElements()
{
	// recalculate vertex size
	mSize = 0;
	for (size_t i=0; i<mElements.size(); ++i) {
		mSize += mElements[i].size();
	}

	return mSize;
}

VertexElement& VertexBuffer::addVertexElement()
{
	mElements.push_back(VertexElement());
	return mElements.back();
}

int VertexBuffer::vertexElementCount()
{
	return int(mElements.size());
}

VertexElement* VertexBuffer::vertexElement(int idx)
{
	assert(idx >= 0 && idx < int(mElements.size()));
	if (idx >= 0 && idx < int(mElements.size())) {
		return &mElements[idx];
	}

	return 0;
}