/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VertexShaderD3D9.h"
#include "Gfx/Submesh.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include <sstream>
#include <set>
#include <assert.h>

namespace Teardrop {
	namespace Gfx {
		namespace Direct3D9 {

			VertexShader::VertexShader(IDirect3DDevice9* device)
				: mDevice(device)
				, mVS(0)
			{
				assert(mDevice);
			}

			VertexShader::~VertexShader()
			{
			}

			bool VertexShader::initialize(Submesh* submesh)
			{
				// build and compile vertex shader

				// start with the output struct
				if (!mSource.length()) {
					mSource.append(
						"struct VSOUT\n"
						"{\n"
						"    float4 HPOS : POSITION;\n"
						"    float4 NORM : TEXCOORD0;\n"
						"    float4 TXC0 : TEXCOORD1;\n"
						"    float4 TXC1 : TEXCOORD2;\n"
						"    float4 TXC2 : TEXCOORD3;\n"
						"    float4 TXC3 : TEXCOORD4;\n"
						"    float4 TXC4 : TEXCOORD5;\n"
						"    float4 TXC5 : TEXCOORD6;\n"
						"    float4 TXC6 : TEXCOORD7;\n"
						"    float4 TXC7 : TEXCOORD8;\n"
						"    float4 TXC8 : TEXCOORD9;\n"
						"};\n"
						);

					// input structs
					mSource.append("struct VSIN\n{\n");

					// generate inputs from Submesh components
					int nVB = submesh->vertexBufferCount();
					for (int i=0; i<nVB; ++i) {
						VertexBuffer* vb = submesh->vertexBuffer(i);
						if (vb) {
							int nElem = vb->vertexElementCount();
							for (int e=0; e<nElem; ++e) {
								VertexElement* elem = vb->vertexElement(e);
								if (elem) {
									switch (elem->mUsage) {
										case VEU_POSITION:
											mSource.append("    float4 POS : POSITION;\n");
											break;
										case VEU_NORMAL:
											mSource.append("    float4 NORM : NORMAL;\n");
											break;
										case VEU_COLOR:
											mSource.append("    float4 COLOR : COLOR0;\n");
											break;
										case VEU_TANGENT:
											mSource.append("    float4 TANGENT : TANGENT;\n");
											break;
										case VEU_BINORMAL:
											mSource.append("    float4 BINORM : BINORMAL;\n");
											break;
										case VEU_BLENDINDEX:
											mSource.append("    float4 BLENDIDX : BLENDINDICES;\n");
											break;
										case VEU_BLENDWEIGHT:
											mSource.append("    float4 BLENDWT : BLENDWEIGHT;\n");
											break;
										case VEU_TEXCOORD:
											if (elem->mIndex == 0)
												mSource.append("    float4 TXC0 : TEXCOORD0;\n");
											else if (elem->mIndex == 1)
												mSource.append("    float4 TXC1 : TEXCOORD1;\n");
											else if (elem->mIndex == 2)
												mSource.append("    float4 TXC2 : TEXCOORD2;\n");
											else if (elem->mIndex == 3)
												mSource.append("    float4 TXC3 : TEXCOORD3;\n");
											break;
										default:
											break;
									}
								}
							}
						}
					}

					mSource.append("};\n\n");

					// then the function definitions

					// then finally the shader body with function calls
					mSource.append("void VS(in VSIN vsin, out VSOUT vsout)\n{\n");
					
					// start with clearing out the VSOUT members
					mSource.append(
						"    vsout.HPOS = float4(0,0,0,0);\n"
						"    vsout.NORM = float4(0,0,0,0);\n"
						"    vsout.TXC0 = float4(0,0,0,0);\n"
						"    vsout.TXC1 = float4(0,0,0,0);\n"
						"    vsout.TXC2 = float4(0,0,0,0);\n"
						"    vsout.TXC3 = float4(0,0,0,0);\n"
						"    vsout.TXC4 = float4(0,0,0,0);\n"
						"    vsout.TXC5 = float4(0,0,0,0);\n"
						"    vsout.TXC6 = float4(0,0,0,0);\n"
						"    vsout.TXC7 = float4(0,0,0,0);\n"
						"    vsout.TXC8 = float4(0,0,0,0);\n\n"
						);

					// closing brace, and we're done
					mSource.append("}\n");
				}

				mHash = submesh->hash();

				if (mSource.length() && !mVS) {
					// compile the shader
					LPD3DXBUFFER pErrorMsgs;
					LPD3DXBUFFER pShader;
					LPD3DXCONSTANTTABLE pConstants;

					HRESULT hr = D3DXCompileShader(
						mSource,
						mSource.length(),
						NULL, // no defines
						NULL, // no includes
						"VS", // entry point function
						"vs_3_0", // Shader Model 3.0
						0, // flags
						&pShader,
						&pErrorMsgs,
						&pConstants // constant table
						);

					if (hr != D3D_OK) {
						if (pErrorMsgs) {
							char* pMsgs = (char*)pErrorMsgs->GetBufferPointer();
							pErrorMsgs->Release();
						}
					}
					else {
						hr = mDevice->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &mVS);
						if (hr != D3D_OK) {
							return false;
						}
					}

					if (pShader)
						pShader->Release();
				}

				return true;
			}

			bool VertexShader::destroy()
			{
				// release pixel shader, if initialized
				if (mVS) {
					mVS->Release();
					mVS = 0;
				}

				return true;
			}

			void VertexShader::apply()
			{
				assert(mDevice);

				// check to see if PS needs initialized
				if (mVS && mDevice) {
					mDevice->SetVertexShader(mVS);
				}
			}

			const char* VertexShader::HLSL_COMMON =
				"// This shader is autogenerated\n"
				"\n"
				"typedef float4 RGBA;\n"
				"typedef float3 RGB;\n"
				"\n"
				;

		} // Direct3D9
	} // Gfx
} // Teardrop
