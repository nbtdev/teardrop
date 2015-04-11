/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VertexShaderD3D11.h"
#include "Gfx/Exception.h"
#include "Gfx/ShaderConstantTable.h"
#include "Gfx/ShaderConstant.h"
#include "Gfx/Submesh.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include "Util/_String.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include <assert.h>
#include <list>
#include <set>
#include <sstream>

namespace {
	const char* sSemanticLut[] = {
		"FOG",         // VEU_UNKNOWN,
		"SV_POSITION", // VEU_POSITION,
		"NORMAL",      // VEU_NORMAL,
		"FOG",         // VEU_BLENDWEIGHT,
		"FOG",         // VEU_BLENDINDEX,
		"TEXCOORD",    // VEU_TEXCOORD,
		"NORMAL",      // VEU_TANGENT,
		"NORMAL",      // VEU_BINORMAL,
		"COLOR"        // VEU_COLOR,
	};

	const char* sTypeLut[] = {
		"float",  // VEt_UNKNOWN,
		"byte",   // VEU_BYTE,
		"short",  // VEU_SHORT,
		"half",   // VEU_HALF,
		"float",  // VEU_FLOAT,
	};
} // namespace 

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

struct InterStageElem {
	InterStageElem();
	~InterStageElem();
	char mName[256];
	VertexElementUsage mSemantic = VEU_UNKNOWN;
	VertexElementType mType = VET_UNKNOWN;
	int mWidth = 0;
	int mRank = 0;
	int mIndex = 0;
	bool mSystem = false;
};

InterStageElem::InterStageElem()
{
	ZeroMemory(mName, sizeof(mName));
}

InterStageElem::~InterStageElem()
{
}

class ShaderInterStage{
public:
	ShaderInterStage(const String& aName);
	~ShaderInterStage();

	void addElement(const InterStageElem& aElem);
	int elementCount();
	void exportHLSLDeclaration(String& aSource);

protected:
	std::list<InterStageElem> mElems;
	String mName;
};

ShaderInterStage::ShaderInterStage(const String& aName)
	: mName(aName)
{
}

ShaderInterStage::~ShaderInterStage()
{
}

void ShaderInterStage::addElement(const InterStageElem& aElem)
{
	mElems.push_back(aElem);
}

int ShaderInterStage::elementCount()
{
	return int(mElems.size());
}

void ShaderInterStage::exportHLSLDeclaration(String& aSource)
{
	aSource.append("struct ");
	aSource.append(mName);
	aSource.append(" {\n");

	for (auto& e : mElems) {
		char buf[1024];

		const char* semantic = sSemanticLut[e.mSemantic];
		const char* type = sTypeLut[e.mType];

		if (e.mSemantic == VEU_TEXCOORD) {
			sprintf_s(buf, 1024, "    %s%d %s%d: %s%d;\n", type, e.mWidth, e.mName, e.mIndex, semantic, e.mIndex);
		} else {
			sprintf_s(buf, 1024, "    %s%d %s: %s%d;\n", type, e.mWidth, e.mName, semantic, e.mIndex);
		}

		aSource.append(buf);
	}

	aSource.append("};\n");
}

static const char* sDecls =
	"// This shader is auto-generated\n"
	"\n"
	"cbuffer XformConstants : register(b0) {\n"
	"    float4x4 WorldITXf;\n"
	"    float4x4 WvpXf;\n"
	"    float4x4 WorldXf;\n"
	"    float4x4 WorldInv;\n"
	"    float4x4 ViewIXf;\n"
	"    float4x4 ViewProj;\n"
	"};\n"
	"cbuffer MatrixPalette : register(b1){\n"
	"    float4 Bones[4096];\n"
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

VertexShader::VertexShader(ComPtr<ID3D11Device> aDevice, ShaderConstantTable* constants, Submesh* aSubmesh)
	: mDevice(aDevice)
	//, mVS(0)
	//, mConstantTable(0)
	//, mConstants(constants)
{
	assert(mDevice);

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

		// also set up output data as we go
		ShaderInterStage sis("VSOUT");

		// set up some passthroughs
		String passThru;

		// generate inputs from Submesh components
		int nVB = aSubmesh->vertexBufferCount();
		int nNormal = 0;
		int nTexCoord = 0;
		for (int i = 0; i<nVB; ++i) {
			VertexBuffer* vb = aSubmesh->vertexBuffer(i);
			if (vb) {
				int nElem = vb->vertexElementCount();
				for (int e = 0; e<nElem; ++e) {
					VertexElement* elem = vb->vertexElement(e);
					if (elem) {
						InterStageElem ise;
						ise.mSemantic = elem->mUsage;
						ise.mType = elem->mType;
						ise.mWidth = elem->mCount;

						switch (elem->mUsage) {
						case VEU_POSITION:
							mSource.append("    float4 POS : POSITION;\n");
							strcpy_s(ise.mName, sizeof(ise.mName), "POSITION");
							sis.addElement(ise);
							bits |= VEU_POSITION_MASK;
							break;
						case VEU_NORMAL:
							mSource.append("    float4 NORM : NORMAL;\n");
							strcpy_s(ise.mName, sizeof(ise.mName), "NORMAL");
							ise.mIndex = nNormal++;
							sis.addElement(ise);
							bits |= VEU_NORMAL_MASK;
							break;
						case VEU_COLOR:
							mSource.append("    float4 COLOR : COLOR0;\n");
							strcpy_s(ise.mName, sizeof(ise.mName), "COLOR");
							sis.addElement(ise);
							bits |= VEU_COLOR_MASK;
							break;
						case VEU_TANGENT:
							mSource.append("    float4 TANGENT : TANGENT;\n");
							strcpy_s(ise.mName, sizeof(ise.mName), "TANGENT");
							ise.mIndex = nNormal++;
							sis.addElement(ise);
							bits |= VEU_TANGENT_MASK;
							break;
						case VEU_BINORMAL:
							mSource.append("    float4 BINORM : BINORMAL;\n");
							strcpy_s(ise.mName, sizeof(ise.mName), "BINORM");
							ise.mIndex = nNormal++;
							sis.addElement(ise);
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
							{
								char buf[64];
								sprintf_s(buf, sizeof(buf), "    %s%d TXC%d : TEXCOORD%d;\n", sTypeLut[elem->mType], elem->mCount, elem->mIndex, elem->mIndex);
								mSource.append(buf);
								sprintf_s(buf, sizeof(buf), "    vsout.TEXCOORD%d = vsin.TXC%d;\n", elem->mIndex, elem->mIndex);
								passThru.append(buf);
							}

							strcpy_s(ise.mName, sizeof(ise.mName), "TEXCOORD");
							ise.mIndex = nTexCoord++;
							sis.addElement(ise);
							break;
						default:
							break;
						}
					}
				}
			}
		}

		mSource.append("};\n\n");

		// export the source for the VSOUT struct
		sis.exportHLSLDeclaration(mSource);

		// then finally the shader body with function calls
		mSource.append("void VS(in VSIN vsin, out VSOUT vsout)\n{\n");

		// start with passing through to the VSOUT members
		mSource.append("    float oo255 = 1.0f / 255.0f;\n");
		mSource.append(passThru);

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
				"    vsout.NORMAL = mul(vsin.NORM, WorldITXf);\n"
				"    float4 Po = float4(vsin.POS.xyz, 1);\n"
				"    float3 Pw = mul(Po,WorldXf).xyz;\n"
				"    //OUT.LightVec = (Lamp0Pos[0] - Pw);\n"
				"\n"
				"    //OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);\n"
				"    vsout.POSITION = mul(Po,WvpXf);\n"
				);
		}

		// closing brace, and we're done
		mSource.append("}\n");
	}

	mHash = aSubmesh->hash();

#if defined(_DEBUG) || defined(DEBUG)
	Environment::get().pLogger->logMessage(mSource);
#endif

	if (mSource.length() && !mVS) {
		// compile the shader
		ComPtr<ID3DBlob> errMsgs;

		HRESULT hr = D3DCompile(
			mSource,
			mSource.length(),
			nullptr,
			nullptr, // no defines
			nullptr, // no includes
			"VS", // entry point function
			"vs_5_0", // Shader Model 5.0
			0, // flags1 (compilie options)
			0, // flags2 (effect compile options)
			&mBytecode,
			&errMsgs);

		if (FAILED(hr)) {
			if (errMsgs) {
				mErrs = (const char*)errMsgs->GetBufferPointer();

				// throw
				throw ShaderCompilationException("Failed to compile vertex shader", mSource, mErrs);
			}
		}
		else {
			hr = mDevice->CreateVertexShader(
				mBytecode->GetBufferPointer(),
				mBytecode->GetBufferSize(),
				mLinkage.Get(),
				&mVS
				);

			if (FAILED(hr)) {
				throw Exception("Could not create vertex shader");
			}
		}
	}
}

VertexShader::~VertexShader()
{
}

void VertexShader::apply()
{
	ComPtr<ID3D11DeviceContext> ctx;
	mDevice->GetImmediateContext(&ctx);
	ctx->VSSetShader(mVS.Get(), nullptr, 0);
}

void* VertexShader::bytecode()
{
	if (mBytecode) {
		return mBytecode->GetBufferPointer();
	}

	return nullptr;
}

int VertexShader::bytecodeLength()
{
	if (mBytecode) {
		return mBytecode->GetBufferSize();
	}

	return 0;
}


} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
