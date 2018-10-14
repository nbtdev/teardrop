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

#include "stdafx.h"
#include "VertexDeclarationD3D9.h"
#include "Gfx/Submesh.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include <assert.h>

namespace Teardrop {
	namespace Gfx {
		namespace Direct3D9 {

			VertexDeclaration::VertexDeclaration(IDirect3DDevice9* device, Submesh* parent)
				: Gfx::VertexDeclaration(parent)
				, mDevice(device)
				, mDecl(0)
				, mElements(0)
			{
				assert(mDevice);
				if (mDevice)
					mDevice->AddRef();
			}

			VertexDeclaration::~VertexDeclaration()
			{
				if (mDecl)
					mDecl->Release();

				delete [] mElements;

				if (mDevice)
					mDevice->Release();
			}

			static D3DDECLTYPE makeD3D9Type(VertexElementType type, int count)
			{
				if (type == VET_FLOAT) {
					if (count == 4) return D3DDECLTYPE_FLOAT4;
					else if (count == 2) return D3DDECLTYPE_FLOAT2;
					else if (count == 1) return D3DDECLTYPE_FLOAT1;
					if (count == 3) return D3DDECLTYPE_FLOAT3;
				}
				else if (type == VET_BYTE && count == 4) 
					return D3DDECLTYPE_UBYTE4;
				else if (type == VET_SHORT) {
					if (count == 2) return D3DDECLTYPE_SHORT2;
					else if (count == 4) return D3DDECLTYPE_SHORT4;
				}
				else if (type == VET_HALF) {
					if (count == 2) return D3DDECLTYPE_FLOAT16_2;
					else if (count == 4) return D3DDECLTYPE_FLOAT16_4;
				}

				return D3DDECLTYPE_UNUSED;
			}

			static D3DDECLUSAGE s_usageLUT[] = 
			{
				D3DDECLUSAGE_FOG,           // VEU_UNKNOWN,
				D3DDECLUSAGE_POSITION,		// VEU_POSITION,
				D3DDECLUSAGE_NORMAL,		// VEU_NORMAL,
				D3DDECLUSAGE_BLENDWEIGHT,	// VEU_BLENDWEIGHT,
				D3DDECLUSAGE_BLENDINDICES,	// VEU_BLENDINDEX,
				D3DDECLUSAGE_TEXCOORD,		// VEU_TEXCOORD,
				D3DDECLUSAGE_TANGENT,		// VEU_TANGENT,
				D3DDECLUSAGE_BINORMAL,		// VEU_BINORMAL,
				D3DDECLUSAGE_COLOR,			// VEU_COLOR,
			};

			void VertexDeclaration::rebuild()
			{
				mHash = 0;
				int nTexCoord = 0;

				int nElem = 0;
				int nVB = mParent->vertexBufferCount();
				for (int v=0; v<nVB; ++v) {
					VertexBuffer* vb = mParent->vertexBuffer(v);
					if (vb) {
						nElem += vb->vertexElementCount();
					}
				}

				delete [] mElements;
				mElements = TD_NEW D3DVERTEXELEMENT9[nElem+1];
				ZeroMemory(mElements, sizeof(D3DVERTEXELEMENT9)*(nElem+1));

				int eIdx = 0;
				for (int v=0; v<nVB; ++v) {
					VertexBuffer* vb = mParent->vertexBuffer(v);

					if (vb) {
						int offset = 0;

						int nVbElem = vb->vertexElementCount();
						for (int e=0; e<nVbElem; ++e)
						{
							VertexElement* ve = vb->vertexElement(e);
							D3DVERTEXELEMENT9& elem = mElements[eIdx];

							elem.Method = D3DDECLMETHOD_DEFAULT;
							elem.Type = (BYTE)makeD3D9Type(ve->mType, ve->mCount);
							elem.Usage = (BYTE)s_usageLUT[ve->mUsage];
							elem.UsageIndex = (BYTE)ve->mIndex;
							elem.Offset = (WORD)offset;
							elem.Stream = (WORD)v;

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

				// mark the end of the decl list
				mElements[eIdx].Stream = 0xFF;
				mElements[eIdx].Offset = 0;
				mElements[eIdx].Type = D3DDECLTYPE_UNUSED;
				mElements[eIdx].Method = 0;
				mElements[eIdx].Usage = 0;
				mElements[eIdx].UsageIndex = 0;

				// release any existing declaration object
				if (mDecl) {
					mDecl->Release();
					mDecl = 0;
				}

				// create the vertex declaration
				mDevice->CreateVertexDeclaration(mElements, &mDecl);
			}

			IDirect3DVertexDeclaration9* VertexDeclaration::declaration()
			{
				return mDecl;
			}

		}
	} // namespace Gfx
} // namespace Teardrop
