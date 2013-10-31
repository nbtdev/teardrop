/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "StaticMeshAsset.h"
#include "Gfx/Mesh.h"
#include "Gfx/Submesh.h"
#include "Gfx/IndexBuffer.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include "Stream/Stream.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(StaticMeshAsset);

StaticMeshAsset::StaticMeshAsset()
	: mMesh(0)
{
}

StaticMeshAsset::~StaticMeshAsset()
{
	delete mMesh;
}

int StaticMeshAsset::serialize(Stream& strm)
{
	if (!mMesh)
		return 0;

	int nBytes = 0;

	int nSubmesh = mMesh->submeshCount();
	nBytes += strm.write(&nSubmesh, sizeof(nSubmesh));

	for (int s=0; s<nSubmesh; ++s) {
		Submesh* sm = mMesh->submesh(s);

		// write primitive type
		int primType = sm->primitiveType();
		nBytes += strm.write(&primType, sizeof(primType));

		IndexBuffer* ib = sm->indexBuffer();
		if (ib && ib->indexCount()) {
			// write index count
			int nIndices = ib->indexCount();
			nBytes += strm.write(&nIndices, sizeof(nIndices));

			// index size
			int indexSize = ib->indexSize();
			nBytes += strm.write(&indexSize, sizeof(indexSize));

			// then the actual index data
			int byteLen = nIndices * indexSize;
			void* data = ib->map(IndexBuffer::MAP_READONLY);
			nBytes += strm.write(data, byteLen);
			ib->unmap();
		}
		else {
			// just write zero for index count
			int zero = 0;
			nBytes += strm.write(&zero, sizeof(zero));
		}

		// then the vertex data
		int nVB = sm->vertexBufferCount();
		nBytes += strm.write(&nVB, sizeof(nVB));

		for (int vbi=0; vbi<nVB; ++vbi) {
			VertexBuffer* vb = sm->vertexBuffer(vbi);
			
			int nElems = vb->vertexElementCount();
			nBytes += strm.write(&nElems, sizeof(nElems));

			for (int e=0; e<nElems; ++e) {
				Gfx::VertexElement* elem = vb->vertexElement(e);

				int type = elem->mType;
				nBytes += strm.write(&type, sizeof(type));
				nBytes += strm.write(&elem->mCount, sizeof(elem->mCount));
				int usage = elem->mUsage;
				nBytes += strm.write(&usage, sizeof(usage));
				nBytes += strm.write(&elem->mIndex, sizeof(elem->mIndex));
			}

			// vertex count
			int nVerts = vb->vertexCount();
			nBytes += strm.write(&nVerts, sizeof(nVerts));

			// then the actual VB data
			void* data = vb->map(VertexBuffer::MAP_READONLY);
			nBytes += strm.write(data, vb->vertexSize() * nVerts);
		}
	}

	return nBytes;
}

int StaticMeshAsset::deserialize(Stream& strm)
{
	int nBytes = 0;

	// read submesh count -- if zero, skip out
	int nSubmesh;
	nBytes += strm.read(&nSubmesh, sizeof(nSubmesh));

	if (!nSubmesh)
		return nBytes;

	// otherwise, make a new mesh (deleting existing 
	// one first if necessary) and populate it up
	delete mMesh;
	mMesh = TD_NEW Mesh;
	mMesh->createSubmeshes(nSubmesh);

	for (int s=0; s<nSubmesh; ++s) {
		Submesh* sm = mMesh->submesh(s);

		// read primitive type
		int primType;
		nBytes += strm.read(&primType, sizeof(primType));
		sm->setPrimitiveType((Submesh::PrimitiveType)primType);

		// read index count; if positive, create an index buffer and populate it
		int nIndices;
		nBytes += strm.read(&nIndices, sizeof(nIndices));

		if (nIndices) {
			IndexBuffer* ib = sm->createIndexBuffer();
			
			int indexSize;
			nBytes += strm.read(&indexSize, sizeof(indexSize));

			ib->initialize(indexSize, nIndices);

			void* data = ib->map(IndexBuffer::MAP_DISCARD);
			nBytes += strm.read(data, indexSize * nIndices);
			ib->unmap();
		}

		// then vertex buffer count
		int nVB;
		nBytes += strm.read(&nVB, sizeof(nVB));

		for (int v=0; v<nVB; ++v) {
			VertexBuffer* vb = sm->createVertexBuffer();

			// vertex elements first
			int nElems;
			nBytes += strm.read(&nElems, sizeof(nElems));

			vb->beginAddVertexElements();
			for (int e=0; e<nElems; ++e) {
				VertexElement& elem = vb->addVertexElement();

				int type;
				nBytes += strm.read(&type, sizeof(type));
				elem.mType = (VertexElementType)type;
				nBytes += strm.read(&elem.mCount, sizeof(elem.mCount));
				int usage;
				nBytes += strm.read(&usage, sizeof(usage));
				elem.mUsage = (VertexElementUsage)usage;
				nBytes += strm.read(&elem.mIndex, sizeof(elem.mIndex));
			}

			int vertSize = vb->endAddVertexElements();

			// then the vertex data
			int nVerts;
			nBytes += strm.read(&nVerts, sizeof(nVerts));
			vb->initialize(nVerts, VertexBuffer::INIT_STATIC|VertexBuffer::INIT_WRITEONLY);
			void* data = vb->map(VertexBuffer::MAP_DISCARD);
			nBytes += strm.read(data, nVerts * vertSize);
			vb->unmap();
		}
	}

	return nBytes;
}

Gfx::Mesh* StaticMeshAsset::mesh()
{
	if (!mMesh)
		mMesh = TD_NEW Gfx::Mesh;

	return mMesh;
}