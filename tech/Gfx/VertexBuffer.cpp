/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/


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
