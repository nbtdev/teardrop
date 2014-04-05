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
				, mConstantTable(0)
			{
				assert(mDevice);
			}

			VertexShader::~VertexShader()
			{
				if (mConstantTable)
					mConstantTable->Release();
			}

			static const char* sDecls = 
				"// This shader is auto-generated\n"
				"\n"
				"float4x4 WorldITXf : WORLDINVTRANS;\n"
				"float4x4 WvpXf : WORLDVIEWPROJ;\n"
				"float4x4 WorldXf : WORLD;\n"
				"float4x4 WorldInv : WORLDINV;\n"
				"float4x4 ViewIXf : VIEWINV;\n"
				"float4x4 ViewProj : VIEWPROJ;\n"
				"float4 Bones[208] : MATRIXPALETTE;\n"
				"\n"
				"struct VSOUT\n"
				"{\n"
				"    float4 HPOS : POSITION;\n"
				"    float4 COLOR : COLOR0;\n"
				"    float4 NORM : TEXCOORD0;\n"
				"    float4 TXC0 : TEXCOORD1;\n"
				"    float4 TXC1 : TEXCOORD2;\n"
				"    float4 TXC2 : TEXCOORD3;\n"
				"    float4 TXC3 : TEXCOORD4;\n"
				"    float4 TXC4 : TEXCOORD5;\n"
				"    float4 TXC5 : TEXCOORD6;\n"
				"    float4 TXC6 : TEXCOORD7;\n"
				"    float4 TXC7 : TEXCOORD8;\n"
				"};\n";

			static const char* sFuncs = 
				"float4 doSkin(float4 pos, const float bone, const float wt)\n"
				"{\n"
				"	float4 rtn;\n"
				"	int c = bone * 3;\n"
				"	rtn.x = dot(Bones[c], pos);\n"
				"	rtn.y = dot(Bones[c+1], pos);\n"
				"	rtn.z = dot(Bones[c+2], pos);\n"
				"	rtn.w = pos.w;\n"
				"	return rtn * wt;\n"
				"}\n"
				"\n"
				"float4 doSkinning(float4 pos, float4 ind, float4 wt)\n"
				"{ \n"
				"	float4 rtn;\n"
				"	rtn  = doSkin(pos, ind.x, wt.x);\n"
				"	rtn += doSkin(pos, ind.y, wt.y);\n"
				"	rtn += doSkin(pos, ind.z, wt.z);\n"
				"	rtn += doSkin(pos, ind.w, wt.w);\n"
				"	return rtn;\n"
				"}\n";

			bool VertexShader::initialize(Submesh* submesh)
			{
				// build and compile vertex shader

				// start with the output struct
				if (!mSource.length()) {
					// boilerplate and function defs
					mSource.append(sDecls);
					mSource.append(sFuncs);

					// track which bits we need to process
					unsigned int bits = 0;

					// input structs
					mSource.append("struct VSIN\n{\n");

					const int VEU_POSITION_MASK = (1 << VEU_POSITION);
					const int VEU_COLOR_MASK = (1 << VEU_COLOR);
					const int VEU_NORMAL_MASK = (1 << VEU_NORMAL);
					const int VEU_BINORMAL_MASK = (1 << VEU_BINORMAL);
					const int VEU_TANGENT_MASK = (1 << VEU_TANGENT);
					const int VEU_BLENDINDEX_MASK = (1 << VEU_BLENDINDEX);
					const int VEU_BLENDWEIGHT_MASK = (1 << VEU_BLENDWEIGHT);
					const int VEU_TEXCOORD_MASK = (1 << VEU_TEXCOORD);
					const int VEU_TEXCOORD0_MASK = VEU_TEXCOORD_MASK | (1 << 16);
					const int VEU_TEXCOORD1_MASK = VEU_TEXCOORD_MASK | (1 << 17);
					const int VEU_TEXCOORD2_MASK = VEU_TEXCOORD_MASK | (1 << 18);
					const int VEU_TEXCOORD3_MASK = VEU_TEXCOORD_MASK | (1 << 19);

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
											bits |= VEU_POSITION_MASK;
											break;
										case VEU_NORMAL:
											mSource.append("    float4 NORM : NORMAL;\n");
											bits |= VEU_NORMAL_MASK;
											break;
										case VEU_COLOR:
											mSource.append("    float4 COLOR : COLOR0;\n");
											bits |= VEU_COLOR_MASK;
											break;
										case VEU_TANGENT:
											mSource.append("    float4 TANGENT : TANGENT;\n");
											bits |= VEU_TANGENT_MASK;
											break;
										case VEU_BINORMAL:
											mSource.append("    float4 BINORM : BINORMAL;\n");
											bits |= VEU_BINORMAL_MASK;
											break;
										case VEU_BLENDINDEX:
											mSource.append("    float4 BLENDIDX : BLENDINDICES;\n");
											bits |= VEU_BLENDINDEX_MASK;
											break;
										case VEU_BLENDWEIGHT:
											mSource.append("    float4 BLENDWT : BLENDWEIGHT;\n");
											bits |= VEU_BLENDWEIGHT_MASK;
											break;
										case VEU_TEXCOORD:
											bits |= ((1 << VEU_TEXCOORD) | (1 << (16 + elem->mIndex)));
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

					// then finally the shader body with function calls
					mSource.append("void VS(in VSIN vsin, out VSOUT vsout)\n{\n");
					
					// start with clearing out the VSOUT members
					mSource.append(
						"    vsout.HPOS = float4(0,0,0,0);\n"
						"    vsout.COLOR = float4(0,0,0,0);\n"
						"    vsout.NORM = float4(0,0,0,0);\n"
						"    vsout.TXC0 = float4(0,0,0,0);\n"
						"    vsout.TXC1 = float4(0,0,0,0);\n"
						"    vsout.TXC2 = float4(0,0,0,0);\n"
						"    vsout.TXC3 = float4(0,0,0,0);\n"
						"    vsout.TXC4 = float4(0,0,0,0);\n"
						"    vsout.TXC5 = float4(0,0,0,0);\n"
						"    vsout.TXC6 = float4(0,0,0,0);\n"
						"    vsout.TXC7 = float4(0,0,0,0);\n"
						"    float oo255 = 1.0f / 255.0f;\n"
						);

					if ((bits & VEU_TEXCOORD0_MASK) == VEU_TEXCOORD0_MASK) {
						mSource.append(
							"    vsout.TXC0 = vsin.TXC0;\n"
							);
					}

					if ((bits & VEU_TEXCOORD1_MASK) == VEU_TEXCOORD0_MASK) {
						mSource.append(
							"    vsout.TXC1 = vsin.TXC1;\n"
							);
					}

					if ((bits & VEU_TEXCOORD2_MASK) == VEU_TEXCOORD0_MASK) {
						mSource.append(
							"    vsout.TXC2 = vsin.TXC2;\n"
							);
					}

					if ((bits & VEU_TEXCOORD3_MASK) == VEU_TEXCOORD0_MASK) {
						mSource.append(
							"    vsout.TXC3 = vsin.TXC3;\n"
							);
					}

					if (bits & VEU_BLENDINDEX_MASK) {
						// then GPU skinning
						mSource.append(
							"    vsout.HPOS = doSkinning(vsin.POS, vsin.BLENDIDX, vsin.BLENDWT*oo255);\n"
							"    vsout.HPOS = mul(vsout.HPOS, ViewProj);\n"
							"    float3 worldNormal = doSkinning(vsin.NORM, vsin.BLENDIDX, vsin.BLENDWT*oo255);\n"
							"    vsout.NORM = mul(WorldInv, worldNormal).xyz;\n"
							"    normalize(vsout.NORM);\n"
							);
					}
					else {
						// static/world geometry
						mSource.append(
							"    vsout.NORM = mul(vsin.NORM, WorldITXf);\n"
							"    float4 Po = vsin.POS;\n"
							"    float3 Pw = mul(Po,WorldXf).xyz;\n"
							"    //OUT.LightVec = (Lamp0Pos[0] - Pw);\n"
							"\n"
							"    //OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);\n"
							"    vsout.HPOS = mul(Po,WvpXf);\n"
							);
					}

					// closing brace, and we're done
					mSource.append("}\n");
				}

				mHash = submesh->hash();

				if (mSource.length() && !mVS) {
					// compile the shader
					LPD3DXBUFFER pErrorMsgs;
					LPD3DXBUFFER pShader;

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
						&mConstantTable// constant table
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

					// wrangle constants used by the shader
					if (mConstantTable) {
						D3DXCONSTANTTABLE_DESC desc;
						if (SUCCEEDED(mConstantTable->GetDesc(&desc))) {
							// bind destination (shader) constants to their renderer (source) constants
							mBindings.resize(desc.Constants);

							for (UINT i=0; i<desc.Constants; ++i) {
								mBindings[i] = 0; // = by constant name, find pointer to entry in renderer's constant table
							}
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
		} // Direct3D9
	} // Gfx
} // Teardrop
