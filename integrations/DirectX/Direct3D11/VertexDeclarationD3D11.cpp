/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "VertexDeclarationD3D11.h"
#include "ShaderManagerD3D11.h"
#include "VertexShaderD3D11.h"
#include "Gfx/Exception.h"
#include "Gfx/Submesh.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

VertexDeclaration::VertexDeclaration(ComPtr<ID3D11Device> device, Submesh* parent)
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

	// it seems pretty ugly from a dependency perspective, but D3D11 forces us to have an 
	// existing vertex shader in order to make a vertex declaration (input format), so we 
	// need to fetch one that matches the submesh (or face the warnings that the D3D runtime
	// will spew at us)
	VertexShader* vs = static_cast<VertexShader*>(ShaderManager::instance().createOrFindInstanceOf(mParent));

	// create the vertex declaration
	HRESULT hr = mDevice->CreateInputLayout(
		mElements,
		nElem,
		vs->bytecode(),
		vs->bytecodeLength(),
		&mLayout
		);

	if (FAILED(hr)) {
		throw Exception("Failed to (re)build vertex input layout in VertexDeclaration::rebuild");
	}
}

ID3D11InputLayout* VertexDeclaration::layout()
{
	if (!mLayout)
		rebuild();

	return mLayout.Get();
}

} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
