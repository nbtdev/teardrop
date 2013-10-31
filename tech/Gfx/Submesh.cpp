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
	, mVertexDeclaration(0)
	, mPrimitiveType(PT_UNKNOWN)
{
}

Submesh::~Submesh()
{
	for (size_t i=0; i<mVertexBuffers.size(); ++i) {
		BufferManager::instance().release(mVertexBuffers[i]);
	}

	BufferManager::instance().release(mIndexBuffer);
	BufferManager::instance().release(mVertexDeclaration);
}

IndexBuffer* Submesh::createIndexBuffer()
{
	assert(mIndexBuffer==0);
	if (mIndexBuffer==0) {
		mIndexBuffer = BufferManager::instance().createIndexBuffer(this);
	}

	return mIndexBuffer;
}

VertexBuffer* Submesh::createVertexBuffer()
{
	mVertexBuffers.push_back(BufferManager::instance().createVertexBuffer(this));
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

VertexDeclaration* Submesh::vertexDeclaration()
{
	if (!mVertexDeclaration) {
		BufferManager::instance().createVertexDeclaration(this);

		// initialize the decl with our vertex layout
	}

	return mVertexDeclaration;
}

void Submesh::clearVertexDeclaration()
{
	if (mVertexDeclaration) {
		BufferManager::instance().release(mVertexDeclaration);
		mVertexDeclaration = 0;
	}
}

void Submesh::setPrimitiveType(PrimitiveType type)
{
	mPrimitiveType = type;
}

Submesh::PrimitiveType Submesh::primitiveType()
{
	return mPrimitiveType;
}
