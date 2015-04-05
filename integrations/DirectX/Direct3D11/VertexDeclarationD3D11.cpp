/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VertexDeclarationD3D11.h"
#include "Gfx/Exception.h"
#include "Gfx/Submesh.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

VertexDeclaration::VertexDeclaration(ID3D11Device* device, Submesh* parent)
	: Gfx::VertexDeclaration(parent)
	, mDevice(device)
{
	assert(mDevice);
}

VertexDeclaration::~VertexDeclaration()
{
	delete[] mElements;
}

namespace {

static DXGI_FORMAT makeD3D11Type(VertexElementType type, int count)
{
	if (type == VET_FLOAT) {
		if (count == 4) return DXGI_FORMAT_R32G32B32A32_FLOAT;
		else if (count == 2) return DXGI_FORMAT_R32G32_FLOAT;
		else if (count == 1) return DXGI_FORMAT_R32_FLOAT;
		else if (count == 3) return DXGI_FORMAT_R32G32B32_FLOAT;
	}
	else if (type == VET_BYTE && count == 4)
		return DXGI_FORMAT_R8_UINT;
	else if (type == VET_SHORT) {
		if (count == 2) return DXGI_FORMAT_R16G16_UINT;
		else if (count == 4) return DXGI_FORMAT_R16G16B16A16_UINT;
	}
	else if (type == VET_HALF) {
		if (count == 2) return DXGI_FORMAT_R16G16_FLOAT;
		else if (count == 4) return DXGI_FORMAT_R16G16B16A16_FLOAT;
	}

	return DXGI_FORMAT_UNKNOWN;
}

const char* s_SemanticNameLUT[] =
{
	"UNKNOWN",                  // VEU_UNKNOWN,
	"POSITION",	                // VEU_POSITION,
	"NORMAL",                   // VEU_NORMAL,
	"BLENDWEIGHT",              // VEU_BLENDWEIGHT,
	"BLENDINDICES",             // VEU_BLENDINDEX,
	"TEXCOORD",                 // VEU_TEXCOORD,
	"TANGENT",                  // VEU_TANGENT,
	"BINORMAL",                 // VEU_BINORMAL,
	"COLOR",                    // VEU_COLOR,
};

} // namespace 

void VertexDeclaration::rebuild()
{
	mHash = 0;
	int nTexCoord = 0;

	int nElem = 0;
	int nVB = mParent->vertexBufferCount();
	for (int v = 0; v<nVB; ++v) {
		VertexBuffer* vb = mParent->vertexBuffer(v);
		if (vb) {
			nElem += vb->vertexElementCount();
		}
	}

	delete[] mElements;
	mElements = TD_NEW D3D11_INPUT_ELEMENT_DESC[nElem];

	int eIdx = 0;
	for (int v = 0; v<nVB; ++v) {
		VertexBuffer* vb = mParent->vertexBuffer(v);

		if (vb) {
			int offset = 0;

			int nVbElem = vb->vertexElementCount();
			for (int e = 0; e<nVbElem; ++e)
			{
				VertexElement* ve = vb->vertexElement(e);
				D3D11_INPUT_ELEMENT_DESC& elem = mElements[eIdx];

				elem.SemanticName = s_SemanticNameLUT[ve->mUsage];
				elem.SemanticIndex = (BYTE)ve->mIndex;
				elem.Format = makeD3D11Type(ve->mType, ve->mCount);
				elem.AlignedByteOffset = (WORD)offset;
				elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				elem.InputSlot = v;
				elem.InstanceDataStepRate = 0;

				offset += ve->size();
				++eIdx;

				// while we at this, create the hash (bitmask) that identifies this vertex structure
				if (ve->mUsage == VEU_TEXCOORD)
					nTexCoord++;

				int mask = 0;
				if (ve->mUsage != VEU_UNKNOWN) {
					mask = 1 << ve->mUsage;
				}

				mHash |= mask;
			}
		}
	}

	// add the number of texcoords (if any) to the hash
	mHash |= (nTexCoord << 16);

	// release any existing declaration object
	mLayout.Reset();

	// create the vertex declaration
	HRESULT hr = mDevice->CreateInputLayout(
		mElements,
		nElem,
		nullptr,
		0,
		&mLayout
		);

	if (FAILED(hr)) {
		throw Exception("Failed to (re)build vertex input layout in VertexDeclaration::rebuild");
	}
}

ID3D11InputLayout* VertexDeclaration::declaration()
{
	return mLayout.Get();
}

}
} // namespace Gfx
} // namespace Teardrop