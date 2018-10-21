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

#include "FragmentShaderD3D11.h"
#include "Texture2DD3D11.h"
#include "Gfx/Exception.h"
#include "Gfx/Material.h"
#include "Gfx/Connection.h"
#include "Gfx/Attribute.h"
#include "Gfx/MaterialExpression.h"
#include "Gfx/MaterialOutput.h"
#include "Gfx/Sampler2DExpression.h"
#include "Gfx/ShaderConstantTable.h"
#include "Math/Vector4.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include <list>
#include <sstream>
#include <set>
#include <assert.h>

namespace {

using namespace Teardrop::Gfx;

const char* sSemanticLut[] = {
	"FOG",      // VEU_UNKNOWN,
	"SV_POSITION", // VEU_POSITION,
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

D3D11_FILTER getFilter(
	Sampler2D::Filter aMin,
	Sampler2D::Filter aMag,
	Sampler2D::Filter aMip)
{
	if (aMin == Sampler2D::FILTER_NONE || aMag == Sampler2D::FILTER_NONE || aMip == Sampler2D::FILTER_NONE)
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	if (aMin == Sampler2D::FILTER_ANISOTROPIC || aMag == Sampler2D::FILTER_ANISOTROPIC || aMip == Sampler2D::FILTER_ANISOTROPIC)
		return D3D11_FILTER_ANISOTROPIC;

	switch (aMin) {
	case Sampler2D::FILTER_NEAREST:
		switch (aMag) {
		case Sampler2D::FILTER_NEAREST:
			switch (aMip) {
			case Teardrop::Gfx::Sampler2D::FILTER_NEAREST: return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case Teardrop::Gfx::Sampler2D::FILTER_BILINEAR: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
			break;
		case Sampler2D::FILTER_BILINEAR:
			switch (aMip) {
			case Teardrop::Gfx::Sampler2D::FILTER_NEAREST: return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case Teardrop::Gfx::Sampler2D::FILTER_BILINEAR: return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			}
			break;
		}
		break;
	case Sampler2D::FILTER_BILINEAR:
		switch (aMag) {
		case Sampler2D::FILTER_NEAREST:
			switch (aMip) {
			case Teardrop::Gfx::Sampler2D::FILTER_NEAREST: return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case Teardrop::Gfx::Sampler2D::FILTER_BILINEAR: return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			}
			break;
		case Sampler2D::FILTER_BILINEAR:
			switch (aMip) {
			case Teardrop::Gfx::Sampler2D::FILTER_NEAREST: return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			case Teardrop::Gfx::Sampler2D::FILTER_BILINEAR: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
			break;
		}
		break;
	}

	return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
}

D3D11_TEXTURE_ADDRESS_MODE sMode[] = {
	D3D11_TEXTURE_ADDRESS_CLAMP,       // ADDRMODE_UNSET = 0,
	D3D11_TEXTURE_ADDRESS_WRAP,        // ADDRMODE_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,      // ADDRMODE_MIRROR,
	D3D11_TEXTURE_ADDRESS_CLAMP,       // ADDRMODE_CLAMP,
	D3D11_TEXTURE_ADDRESS_BORDER,      // ADDRMODE_BORDER,
};

ComPtr<ID3D11SamplerState> getD3D11SamplerState(ID3D11Device* aDevice, Teardrop::Gfx::Sampler2DExpression* aExpr)
{
	Teardrop::Vector4 bColor(1.f, 1.f, 1.f, 1.f);

	CD3D11_SAMPLER_DESC desc(
		getFilter(
			aExpr->getSampler2D().getMinFilter(),
			aExpr->getSampler2D().getMagFilter(),
			aExpr->getSampler2D().getMipMapFilter()
		),
		sMode[aExpr->getSampler2D().getAddressModeU()],
		sMode[aExpr->getSampler2D().getAddressModeV()],
		sMode[aExpr->getSampler2D().getAddressModeW()],
		0, // MIP LoD bias
		1, // max aniostropy
		D3D11_COMPARISON_NEVER,
		(float*)&bColor,
		-D3D11_FLOAT32_MAX,
		D3D11_FLOAT32_MAX
		);

	ComPtr<ID3D11SamplerState> state;
	HRESULT hr = aDevice->CreateSamplerState(&desc, &state);
	if (FAILED(hr)) {
		// TODO: throw? 
	}

	return state;
}

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
        size_t exprCount = mMaterial->expressionCount();
		MaterialExpression** expressions = mMaterial->sortedExpressions();

		std::stringstream defs;
        for (size_t i = 0; i < exprCount; ++i) {
			MaterialExpression* expr = expressions[i];

			const Reflection::ClassDef* classDef = expr->getDerivedClassDef();
			if (uniqueExprs.find(classDef) == uniqueExprs.end()) {
				uniqueExprs.insert(classDef);

				// and then generate the definition for this expression
				if (expr->getDerivedClassDef() == Sampler2DExpression::getClassDef()) {
					Sampler2DExpression* sampExp = static_cast<Sampler2DExpression*>(expr);

					// record the value of the next available index in the array of sampler expressions
                    size_t textureIndex = mSamplerExpressions.size();

					// append this expression to the array of sampler expressions
					mSamplerExpressions.push_back(sampExp);

					// obtain a D3D11SamplerState that corresponds to this Sampler2DExpression (D3D11 kindly manages redundant
					// state creation for us and will return pointers to those that already exist)
					ComPtr<ID3D11SamplerState> state = getD3D11SamplerState(mDevice.Get(), sampExp);

					// check the array of sampler states to see if this one already exists; if so, record its index (default is the next array index)
					int samplerIndex = -1;
					int idx = 0;
					for (auto s : mSamplers) {
						if (s == state) {
							samplerIndex = idx;
							break;
						}

						idx++;
					}

					if (samplerIndex == -1) {
						// then it wasn't found, and we need to add it
						samplerIndex = int(mSamplers.size());
						mSamplers.push_back(state);
					}

					sampExp->appendDefinition(MaterialExpression::SHADER_HLSL5, textureIndex, samplerIndex, defs);
				}
				else {
					expr->appendDefinition(MaterialExpression::SHADER_HLSL5, defs);
				}
			}

			// add interpolant consumption
			// POSITION is required
			fsEnv.addInterpolant(VEU_POSITION, VET_FLOAT, 4);

			// everything else is optional; check the "features" to see what is used
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
        size_t nConnections = mMaterial->connections(0, 0);

		//  there may not be connections yet, so do the following only if connections exist
		if (nConnections) {
			std::vector<Connection*> connections(nConnections);
			mMaterial->connections(&connections[0], nConnections);

			// variable names should be based on the name (type) of the expression
			// that owns the "from" (output) attr; for uniqueness, we'll just assign
			// monotonically-increasing ordinals to each connection
            size_t ord = 0;
            for (size_t i = 0; i < nConnections; ++i) {
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
        for (size_t i = 0; i < exprCount; ++i) {
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

            size_t nameIdx = 0;
			for (size_t j = 0; j < inputAttrs.size(); ++j) {
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
				MaterialExpression::SHADER_HLSL5,
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

	if (mSource.length() && !mPS) {
		// compile the shader
		ComPtr<ID3DBlob> errMsgs;

		HRESULT hr = D3DCompile(
			mSource,
			mSource.length(),
			nullptr,
			nullptr, // no defines
			nullptr, // no includes
			"PS", // entry point function
			"ps_5_0", // Shader Model 5.0
			0, // flags1 (compilie options)
			0, // flags2 (effect compile options)
			&mBytecode,
			&errMsgs);

		if (FAILED(hr)) {
			if (errMsgs) {
				mErrs = (const char*)errMsgs->GetBufferPointer();
			}
		}
		else {
			hr = mDevice->CreatePixelShader(
				mBytecode->GetBufferPointer(),
				mBytecode->GetBufferSize(),
				mLinkage.Get(),
				&mPS
				);

			if (FAILED(hr)) {
				throw Exception("Could not create pixel shader");
			}
		}
	}
}

FragmentShader::~FragmentShader()
{
}

void FragmentShader::apply()
{
	assert(mDevice);
	if (!mDevice)
		return;

	ComPtr<ID3D11DeviceContext> ctx;
	mDevice->GetImmediateContext(&ctx);

	// early-out if PS not here
	// TODO: subsituture in a default one?
	if (!mPS)
		return;

	// set the pixel shader
	ctx->PSSetShader(mPS.Get(), nullptr, 0);

	// set sampler states
	ID3D11SamplerState* samplerStates[16];
	int i = 0;
	for (auto s : mSamplers)
		samplerStates[i++] = s.Get();

    ctx->PSSetSamplers(0, (UINT)mSamplers.size(), samplerStates);
	
	// set the actual texture resources
	ID3D11ShaderResourceView* srv[16];
	i = 0;
	for (auto s : mSamplerExpressions) {
		if (s) {
			Texture2D* tex = static_cast<Texture2D*>(s->getSampler2D().texture());
			if (tex)
				srv[i++] = tex->shaderResourceView().Get();
			else
				srv[i++] = 0;
		}
	}

    ctx->PSSetShaderResources(0, (UINT)mSamplerExpressions.size(), srv);
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
