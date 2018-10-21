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


#include "Submesh.h"
#include "BufferManager.h"
#include "VertexDeclaration.h"
#include "VertexBuffer.h"
#include "VertexElement.h"

using namespace Teardrop;
using namespace Gfx;

Submesh::Submesh()
	: mIndexBuffer(0)
	, mVertexDeclaration(0)
	, mPrimitiveType(PT_UNKNOWN)
	, mHash(0)
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

void Submesh::removeVertexBuffer(size_t index)
{
    assert(index >=0 && index < mVertexBuffers.size());

    if (index >=0 && index < mVertexBuffers.size()) {
		BufferManager::instance().release(mVertexBuffers[index]);
		mVertexBuffers.erase(mVertexBuffers.begin()+index);
	}
}

size_t Submesh::vertexBufferCount()
{
    return mVertexBuffers.size();
}

VertexBuffer* Submesh::vertexBuffer(size_t index)
{
    assert(index >=0 && index < mVertexBuffers.size());

    if (index >=0 && index < mVertexBuffers.size()) {
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
		mVertexDeclaration = BufferManager::instance().createVertexDeclaration(this);

		// initialize the decl with our vertex layout
		mVertexDeclaration->rebuild();
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

unsigned int Submesh::hash()
{
	if (mHash == 0 && !mVertexBuffers.empty()) {
		// create a hash from our structure
		for (VertexBuffers::iterator it = mVertexBuffers.begin(); it != mVertexBuffers.end(); ++it) {
			VertexBuffer* vb = *it;
            size_t nElems = vb->vertexElementCount();
            for (size_t i=0; i<nElems; ++i) {
				VertexElement* elem = vb->vertexElement(i);
				mHash |= (1 << elem->mUsage);

				if (elem->mUsage == VEU_TEXCOORD) {
					mHash |= (elem->mIndex << 16);
				}
			}
		}
	}

	return mHash;
}

const ShaderFeatures& Submesh::features()
{
	if (mFeatures.isEmpty() && !mVertexBuffers.empty()) {
		// build the features struct
		for (VertexBuffers::iterator it = mVertexBuffers.begin(); it != mVertexBuffers.end(); ++it) {
			VertexBuffer* vb = *it;
            size_t nElems = vb->vertexElementCount();
            for (size_t i=0; i<nElems; ++i) {
				VertexElement* elem = vb->vertexElement(i);
				switch(elem->mUsage) {
					case VEU_POSITION:
						mFeatures.setFeature(INTERP_POSITION);
						break;
					case VEU_NORMAL:
						mFeatures.setFeature(INTERP_NORMAL);
						break;
					case VEU_TEXCOORD:
                        mFeatures.setFeature(INTERP_TEXCOORD, (int)elem->mIndex);
						break;
				}
			}
		}
	}

	return mFeatures;
}

void Submesh::clearFeatures()
{
	mFeatures.clear();
}
