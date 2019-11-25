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

#include "MeshAsset.h"
#include "Gfx/IndexBuffer.h"
#include "Gfx/Mesh.h"
#include "Gfx/PipelineStateManager.h"
#include "Gfx/Submesh.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include "Stream/Stream.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(MeshAsset);

MeshAsset::MeshAsset()
    : mMesh(nullptr)
    , mPipeline(nullptr)
    , mPipelineHash(0)
{
}

MeshAsset::~MeshAsset()
{
	delete mMesh;
}

uint64_t MeshAsset::serialize(Stream& strm)
{
	if (!mMesh)
		return 0;

    uint64_t nBytes = 0;

    uint64_t nSubmesh = mMesh->submeshCount();
	nBytes += strm.write(&nSubmesh, sizeof(nSubmesh));

    for (size_t s=0; s<nSubmesh; ++s) {
		Submesh* sm = mMesh->submesh(s);

		// write primitive type
        uint32_t primType = sm->primitiveType();
		nBytes += strm.write(&primType, sizeof(primType));

		IndexBuffer* ib = sm->indexBuffer();
		if (ib && ib->indexCount()) {
			// write index count
            uint32_t nIndices = ib->indexCount();
			nBytes += strm.write(&nIndices, sizeof(nIndices));

			// index size
            uint32_t indexSize = ib->indexSize();
			nBytes += strm.write(&indexSize, sizeof(indexSize));

			// then the actual index data
            uint32_t byteLen = nIndices * indexSize;
			void* data = ib->map(IndexBuffer::MAP_READONLY);
			nBytes += strm.write(data, byteLen);
			ib->unmap();
		}
		else {
			// just write zero for index count
            uint32_t zero = 0;
			nBytes += strm.write(&zero, sizeof(zero));
		}

		// then the vertex data
        size_t nVB = sm->vertexBufferCount();
		nBytes += strm.write(&nVB, sizeof(nVB));

        for (size_t vbi=0; vbi<nVB; ++vbi) {
			VertexBuffer* vb = sm->vertexBuffer(vbi);
			
            size_t nElems = vb->vertexElementCount();
			nBytes += strm.write(&nElems, sizeof(nElems));

            for (size_t e=0; e<nElems; ++e) {
                Gfx::VertexElement const* elem = vb->vertexElement(e);

                uint32_t type = elem->mType;
				nBytes += strm.write(&type, sizeof(type));
				nBytes += strm.write(&elem->mCount, sizeof(elem->mCount));
                uint32_t usage = elem->mUsage;
				nBytes += strm.write(&usage, sizeof(usage));
				nBytes += strm.write(&elem->mIndex, sizeof(elem->mIndex));
			}

			// vertex count
            uint32_t nVerts = (uint32_t)vb->vertexCount();
			nBytes += strm.write(&nVerts, sizeof(nVerts));

			// then the actual VB data
			void* data = vb->map(VertexBuffer::MAP_READONLY);
			nBytes += strm.write(data, vb->vertexSize() * nVerts);
			vb->unmap();
		}
	}

	return nBytes;
}

uint64_t MeshAsset::deserialize(Stream& strm)
{
    uint64_t nBytes = 0;

	// read submesh count -- if zero, skip out
    uint64_t nSubmesh;
	nBytes += strm.read(&nSubmesh, sizeof(nSubmesh));

	if (!nSubmesh)
		return nBytes;

	// otherwise, make a new mesh (deleting existing 
	// one first if necessary) and populate it up
    delete mMesh;
    mMesh = TD_NEW Mesh;
	mMesh->createSubmeshes(nSubmesh);

    for (size_t s=0; s<nSubmesh; ++s) {
		Submesh* sm = mMesh->submesh(s);

		// read primitive type
        uint32_t primType;
		nBytes += strm.read(&primType, sizeof(primType));
		sm->setPrimitiveType((Submesh::PrimitiveType)primType);

		// read index count; if positive, create an index buffer and populate it
        uint32_t nIndices;
		nBytes += strm.read(&nIndices, sizeof(nIndices));

		if (nIndices) {
			IndexBuffer* ib = sm->createIndexBuffer();
			
			// TODO: deprecated, we calculate this based on index count
            uint32_t indexSize;
			nBytes += strm.read(&indexSize, sizeof(indexSize));

			ib->initialize(nIndices, 0);

			void* data = ib->map(IndexBuffer::MAP_DISCARD);
			nBytes += strm.read(data, indexSize * nIndices);
			ib->unmap();
		}

		// then vertex buffer count
        uint32_t nVB;
		nBytes += strm.read(&nVB, sizeof(nVB));

        for (uint32_t v=0; v<nVB; ++v) {
			VertexBuffer* vb = sm->createVertexBuffer();

			// vertex elements first
            uint32_t nElems;
			nBytes += strm.read(&nElems, sizeof(nElems));

			vb->beginAddVertexElements();
            for (size_t e=0; e<nElems; ++e) {
				VertexElement& elem = vb->addVertexElement();

                uint32_t type;
				nBytes += strm.read(&type, sizeof(type));
				elem.mType = (VertexElementType)type;
				nBytes += strm.read(&elem.mCount, sizeof(elem.mCount));
                uint32_t usage;
				nBytes += strm.read(&usage, sizeof(usage));
				elem.mUsage = (VertexElementUsage)usage;
				nBytes += strm.read(&elem.mIndex, sizeof(elem.mIndex));
			}

            uint32_t vertSize = (uint32_t)vb->endAddVertexElements();

			// then the vertex data
            uint32_t nVerts;
			nBytes += strm.read(&nVerts, sizeof(nVerts));
			vb->initialize(nVerts, 0);

			void* data = vb->map(VertexBuffer::MAP_DISCARD);
			nBytes += strm.read(data, nVerts * vertSize);
			vb->unmap();
		}
	}

	return nBytes;
}

Gfx::Mesh* MeshAsset::mesh()
{
	if (!mMesh)
        mMesh = TD_NEW Gfx::Mesh;

	return mMesh;
}

Gfx::Pipeline* MeshAsset::pipeline()
{
    if (mPipeline) {
        return mPipeline;
    }

    Gfx::Mesh* mesh = this->mesh();
    if (mesh->submeshCount() == 0) {
        return nullptr;
    }

    // TODO/NOTE: we assume a single material for all submeshes in the mesh!

    // fetch a matching pipeline for us
    if (mPipelineHash > 0) {
        mPipeline = Gfx::PipelineStateManager::instance().findPipelineState(mPipelineHash);
        return mPipeline;
    }

    PipelineData pipelineData = Gfx::PipelineStateManager::instance().createOrFindPipelineState(mesh->submesh(0), getMaterial());
    mPipeline = pipelineData.pipeline;
    mPipelineHash = pipelineData.hash;

    return mPipeline;
}
