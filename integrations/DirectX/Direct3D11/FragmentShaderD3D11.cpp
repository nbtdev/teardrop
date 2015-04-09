/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "FragmentShaderD3D11.h"
#include "Texture2DD3D11.h"
#include "Gfx/Material.h"
#include "Gfx/Connection.h"
#include "Gfx/Attribute.h"
#include "Gfx/MaterialExpression.h"
#include "Gfx/MaterialOutput.h"
#include "Gfx/Sampler2DExpression.h"
#include "Gfx/ShaderConstantTable.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include <list>
#include <sstream>
#include <set>
#include <assert.h>

namespace {
const char* sSemanticLut[] = {
	"FOG",      // VEU_UNKNOWN,
	"POSITION", // VEU_POSITION,
	"NORMAL",   // VEU_NORMAL,
	"FOG",      // VEU_BLENDWEIGHT,
	"FOG",      // VEU_BLENDINDEX,
	"TEXCOORD", // VEU_TEXCOORD,
	"NORMAL",   // VEU_TANGENT,
	"NORMAL",   // VEU_BINORMAL,
	"COLOR"     // VEU_COLOR,
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

struct Interpolant {
	Interpolant();
	~Interpolant();
	char mName[256];
	VertexElementUsage mSemantic = VEU_UNKNOWN;
	VertexElementType mType = VET_UNKNOWN;
	int mWidth = 0;
	int mRank = 0;
	int mIndex = 0;
	bool mSystem = false;
};

Interpolant::Interpolant()
{
	ZeroMemory(mName, sizeof(mName));
}

Interpolant::~Interpolant()
{
}

// FSEnvironmemt is a place to manage the FS's input needs (which interpolants it needs, etc.)
class FSEnvironment
{
public:
	FSEnvironment();
	~FSEnvironment();

	// if an interpolant with the given smeantic and index already 
	// exists, this method will return that interpolant; otherwise, 
	// this new one is added and returned
	const Interpolant& addInterpolant(VertexElementUsage aUsage, VertexElementType aType, int aWidth=4, int aIndex=0, int aRank=1);

	void exportHLSLPSInput(const String& aName, String& aSource);

private:
	std::list<Interpolant> mInterpolants;
};

FSEnvironment::FSEnvironment()
{
}

FSEnvironment::~FSEnvironment()
{
}

const Interpolant& FSEnvironment::addInterpolant(VertexElementUsage aUsage, VertexElementType aType, int aWidth, int aIndex, int aRank)
{
	for (auto& i : mInterpolants) {
		if (i.mSemantic == aUsage && i.mIndex == aIndex)
			// TODO: check type and do something on mismatch?
			return i;
	}

	// make up the interpolant name based on semantic and index
	char buf[64];
	sprintf_s(buf, sizeof(buf), "%s%d", sSemanticLut[aUsage], aIndex);

	Interpolant i;
	strcpy_s(i.mName, sizeof(i.mName), buf);
	i.mSemantic = aUsage;
	i.mType = aType;
	i.mWidth = aWidth;
	i.mRank = aRank;
	i.mIndex = aIndex;

	mInterpolants.push_back(i);	
	return mInterpolants.back();
}

void FSEnvironment::exportHLSLPSInput(const String& aName, String& aSource)
{
	aSource.append("struct ");
	aSource.append(aName);
	aSource.append("\n{\n");

	for (auto& i : mInterpolants) {
		char buf[1024];

		const char* semantic = sSemanticLut[i.mSemantic];
		const char* type = sTypeLut[i.mType];

		if (i.mRank > 1) {
			sprintf_s(buf, 1024, "    %s%dx%d %s: %s%d;\n", type, i.mWidth, i.mRank, i.mName, semantic, i.mIndex);
		}
		else {
			sprintf_s(buf, 1024, "    %s%d %s: %s%d;\n", type, i.mWidth, i.mName, semantic, i.mIndex);
		}

		aSource.append(buf);
	}

	aSource.append("};\n");
}

FragmentShader::FragmentShader(ComPtr<ID3D11Device> aDevice, ShaderConstantTable* constants, Material* mtl)
	: Gfx::FragmentShader(mtl)
	, mDevice(aDevice)
	//, mPS(0)
	//, mConstants(constants)
{
	assert(mDevice);

	FSEnvironment fsEnv;

	// build and compile pixel shader
	if (!mSource.length()) {
		// generate source from material definition/expressions

		// first the common parts
		mSource.append(HLSL_COMMON);

		// perform a topological sort on the shader expressions, so that we can order 
		// their invocation in order, as well as collect their definitions
		mMaterial->sortExpressions();

		// then functions for each expression in the shader; we can iterate the sorted
		// expressions and put out definitions for each unique expression ClassDef
		std::set<const Reflection::ClassDef*> uniqueExprs;
		int exprCount = mMaterial->expressionCount();
		MaterialExpression** expressions = mMaterial->sortedExpressions();

		std::stringstream defs;
		int samplerIndex = 0;
		for (int i = 0; i<exprCount; ++i) {
			MaterialExpression* expr = expressions[i];
			int origSamplerIndex = samplerIndex;

			const Reflection::ClassDef* classDef = expr->getDerivedClassDef();
			if (uniqueExprs.find(classDef) == uniqueExprs.end()) {
				uniqueExprs.insert(classDef);

				// and then generate the definition for this expression
				expr->appendDefinition(MaterialExpression::SHADER_HLSL, samplerIndex, defs);
				// sanity check: the expression should not have incremented the index by more than one
				assert((samplerIndex <= origSamplerIndex+1) && "expression should not have incremented the index by more than one");
			}

			// special case -- samplers aren't regular constants so
			// track the samplers that this shader uses, so that we can 
			// look them up by name later if needed
			if (expr->getDerivedClassDef() == Sampler2DExpression::getClassDef()) {
				Sampler2DExpression* sampExp = static_cast<Sampler2DExpression*>(expr);
				mSamplers[sampExp->samplerName()] = SamplerEnt(sampExp, origSamplerIndex);
			}

			// add interpolant consumption
			const ShaderFeatures& features = expr->features();
			if (features.mInterpolants & INTERP_TEXCOORD) {
				unsigned short mask = features.mTexcoordMask;

				for (int i = 0; i < 16; ++i) {
					if ((mask & (1 << i))) {
						const Interpolant& interp = fsEnv.addInterpolant(VEU_TEXCOORD, VET_FLOAT, 2, i);
					}
				}
			}
		}

		std::string defStr(defs.str());
		mSource.append(defStr.c_str());

		// declare the input struct (matches the VS output struct in VertexShaderD3D9)
		fsEnv.exportHLSLPSInput("PSIN", mSource);

		// open the fragment shader...
		mSource.append("float4 PS(PSIN psin) : SV_TARGET {\n");

		// then generate the function calls
		typedef std::map<const Attribute*, std::string> AttrToVarName;
		AttrToVarName names;

		// go through all connections and form this map
		int nConnections = mMaterial->connections(0, 0);

		//  there may not be connections yet, so do the following only if connections exist
		if (nConnections) {
			std::vector<Connection*> connections(nConnections);
			mMaterial->connections(&connections[0], nConnections);

			// variable names should be based on the name (type) of the expression
			// that owns the "from" (output) attr; for uniqueness, we'll just assign
			// monotonically-increasing ordinals to each connection
			int ord = 0;
			for (int i = 0; i < nConnections; ++i) {
				std::stringstream ss;
				Gfx::Attribute* out = connections[i]->output();
				MaterialExpression* me = out->mParent;
				ss << me->getDerivedClassDef()->getName() << '_' << ord++ << '_' << out->mName;
				std::string tmp(ss.str());

				names[connections[i]->output()] = tmp;
				names[connections[i]->input()] = tmp;
			}
		}

		// then generate the function calls...
		std::stringstream calls;
		for (int i = 0; i<exprCount; ++i) {
			MaterialExpression* expr = expressions[i];

			// our input attributes are someone else's output attributes, so collect their names
			// into a std::vector of strings
			const MaterialExpression::Attributes& inputAttrs = expr->inputAttributes();
			std::vector<std::string> inputs(inputAttrs.size());

			// special case -- MaterialOutput has no connection for its output attribute, but
			// we know what variable we want to use for it, so insert it manually
			if (expr->getDerivedClassDef() == MaterialOutput::getClassDef()) {
				// there is only one output attribute...
				const MaterialExpression::Attributes& outputAttrs = expr->outputAttributes();
				assert(outputAttrs.size() == 1);
				if (outputAttrs.size() == 1) {
					const Attribute* output = &outputAttrs[0];
					names[output] = "output";
				}
			}

			int nameIdx = 0;
			for (size_t j = 0; j<inputAttrs.size(); ++j) {
				// find this attribute's name in the connection map
				const Attribute* attr = &inputAttrs[j];
				AttrToVarName::iterator name = names.find(attr);

				std::string tmp("float(0)");
				std::string* arg = &tmp;

				if (name != names.end()) {
					// then use the generated name
					arg = &(name->second);
				}
				else {
					// make sure the attribute is actually optional, and if so,
					// use the attribute's default value
					assert(attr->mRequired == Attribute::Optional);

					if (attr->mRequired == Attribute::Optional)
						tmp = attr->mDefault;
				}

				inputs[nameIdx++] = *arg;
			}

			// then we can generate the code for this call
			expr->appendCall(
				MaterialExpression::SHADER_HLSL,
				i,
				inputs,
				names,
				calls
				);
		}

		std::string callStr(calls.str());
		mSource.append(callStr.c_str());

		// and then close the shader
		mSource.append("\nreturn output;\n}\n");
		//mSource.append("\nreturn float4(0.5,0.5,0.5,1);\n}\n");
	}

#if defined(_DEBUG) || defined(DEBUG)
	Environment::get().pLogger->logMessage(mSource);
#endif

#if 0
	if (mSource.length() && !mPS) {
		// compile the shader
		LPD3DXBUFFER pErrorMsgs;
		LPD3DXBUFFER pShader;

		HRESULT hr = D3DXCompileShader(
			mSource,
			mSource.length(),
			NULL, // no defines
			NULL, // no includes
			"PS", // entry point function
			"ps_3_0", // Shader Model 3.0
			0, // flags
			&pShader,
			&pErrorMsgs,
			&mConstantTable // constant table
			);

		if (hr != D3D_OK) {
			if (pErrorMsgs) {
				char* pMsgs = (char*)pErrorMsgs->GetBufferPointer();
				pErrorMsgs->Release();
			}
		}
		else {
			hr = mDevice->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &mPS);
			if (hr != D3D_OK) {
				//return false;
			}
		}

		// wrangle constants used by the shader
		if (mConstantTable) {
			D3DXCONSTANTTABLE_DESC desc;
			if (SUCCEEDED(mConstantTable->GetDesc(&desc))) {
				// bind destination (shader) constants to their renderer (source) constants
				mBindings.resize(desc.Constants);

				for (UINT i = 0; i<desc.Constants; ++i) {
					D3DXHANDLE pConst = mConstantTable->GetConstant(NULL, i);

					if (pConst) {
						UINT tmp = 1;
						D3DXCONSTANT_DESC constDesc;
						mConstantTable->GetConstantDesc(pConst, &constDesc, &tmp);

						mBindings[i].mConstant = mConstants->find(constDesc.Name);

						// when the renderer updates an entry in its table it will increment its version number, so we 
						// can compare this to the renderer's version to see if we need to update the data in the shader
						// TODO : is this actually true?
						mBindings[i].mCurrentVersion = 0;
					}
				}
			}
		}

		if (pShader)
			pShader->Release();
	}
#endif
}

FragmentShader::~FragmentShader()
{
	//if (mConstantTable)
	//	mConstantTable->Release();

	//if (mPS) {
	//	mPS->Release();
	//	mPS = 0;
	//}
}

void FragmentShader::apply()
{
	assert(mDevice);

	// check to see if PS needs initialized
	//if (!mPS) {
	//	initialize();
	//}

	//if (mDevice) {
	//	mDevice->SetPixelShader(mPS);
	//}

	// set any shader constants we have
	//if (mConstantTable) {
	//	D3DXCONSTANTTABLE_DESC desc;
	//	if (SUCCEEDED(mConstantTable->GetDesc(&desc))) {
	//		// set each constant we found during compilation
	//		for (UINT i = 0; i<desc.Constants; ++i) {
	//			D3DXHANDLE pConst = mConstantTable->GetConstant(NULL, i);
	//			if (pConst) {
	//				D3DXCONSTANT_DESC constDesc;
	//				UINT ct = 1;
	//				mConstantTable->GetConstantDesc(pConst, &constDesc, &ct);
	//				switch (constDesc.Type) {
	//				case D3DXPT_SAMPLER2D:
	//				{
	//					Samplers::iterator it = mSamplers.find(constDesc.Name);
	//					if (it != mSamplers.end()) {
	//						Sampler2DExpression* sampExp = it->second;
	//						if (sampExp) {
	//							Texture2D* tex = static_cast<Texture2D*>(sampExp->getSampler2D().texture());
	//							mDevice->SetTexture(constDesc.RegisterIndex, tex->textureObject());
	//						}
	//					}
	//				}
	//				break;
	//				}
	//			}
	//		}
	//	}
	//}
}

const char* FragmentShader::HLSL_COMMON =
	"// This shader is autogenerated\n"
	"\n"
	"typedef float4 RGBA;\n"
	"typedef float3 RGB;\n"
	"\n"
	;

} // Direct3D11
} // Gfx
} // Teardrop
