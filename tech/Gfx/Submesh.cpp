/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Submesh.h"
#include "BufferManager.h"

using namespace Teardrop;
using namespace Gfx;

Submesh::Submesh()
	: mIndexBuffer(0)
{
}

Submesh::~Submesh()
{
	for (size_t i=0; i<mVertexBuffers.size(); ++i) {
		BufferManager::instance().release(mVertexBuffers[i]);
	}

	BufferManager::instance().release(mIndexBuffer);
}

IndexBuffer* Submesh::createIndexBuffer()
{
	assert(mIndexBuffer==0);
	if (mIndexBuffer==0) {
		mIndexBuffer = BufferManager::instance().createIndexBuffer();
	}

	return mIndexBuffer;
}

VertexBuffer* Submesh::createVertexBuffer()
{
	mVertexBuffers.push_back(BufferManager::instance().createVertexBuffer());
	return mVertexBuffers.back();
}

void Submesh::removeIndexBuffer()
{
	BufferManager::instance().release(mIndexBuffer);
	mIndexBuffer = 0;
}

void Submesh::removeVertexBuffer(int index) 
{
	assert(index >=0 && index < int(mVertexBuffers.size()));

	if (index >=0 && index < int(mVertexBuffers.size())) {
		BufferManager::instance().release(mVertexBuffers[index]);
		mVertexBuffers.erase(mVertexBuffers.begin()+index);
	}
}

int Submesh::vertexBufferCount()
{
	return int(mVertexBuffers.size());
}

VertexBuffer* Submesh::vertexBuffer(int index)
{
	assert(index >=0 && index < int(mVertexBuffers.size()));

	if (index >=0 && index < int(mVertexBuffers.size())) {
		return mVertexBuffers[index];
	}

	return 0;
}

IndexBuffer* Submesh::indexBuffer()
{
	return mIndexBuffer;
}
