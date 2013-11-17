/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VertexDeclarationD3D9.h"
#include "Gfx/Submesh.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"

namespace Teardrop {
	namespace Gfx {
		namespace Direct3D9 {

			VertexDeclaration::VertexDeclaration(IDirect3DDevice9* device, Submesh* parent)
				: Gfx::VertexDeclaration(parent)
				, mDevice(device)
				, mDecl(0)
				, mElements(0)
			{
			}

			VertexDeclaration::~VertexDeclaration()
			{
				if (mDecl)
					mDecl->Release();

				delete [] mElements;
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
						}
					}
				}

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
