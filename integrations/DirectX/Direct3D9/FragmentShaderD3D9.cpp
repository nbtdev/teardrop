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

#include "stdafx.h"
#include "FragmentShaderD3D9.h"
#include "Texture2DD3D9.h"
#include "Gfx/Material.h"
#include "Gfx/Connection.h"
#include "Gfx/Attribute.h"
#include "Gfx/MaterialExpression.h"
#include "Gfx/MaterialOutput.h"
#include "Gfx/Sampler2DExpression.h"
#include "Gfx/ShaderConstantTable.h"
#include <sstream>
#include <set>
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

FragmentShader::FragmentShader(IDirect3DDevice9* device, ShaderConstantTable* constants, Material* mtl)
	: Gfx::FragmentShader(mtl)
	, mDevice(device)
	, mPS(0)
	, mConstants(constants)
{
	assert(mDevice);
	if (mDevice)
		mDevice->AddRef();
}

FragmentShader::~FragmentShader()
{
	if (mConstantTable)
		mConstantTable->Release();

	if (mPS)
		mPS->Release();

	if (mDevice)
		mDevice->Release();
}

bool FragmentShader::initialize()
{
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
		for (int i=0; i<exprCount; ++i) {
			MaterialExpression* expr = expressions[i];
			const Reflection::ClassDef* classDef = expr->getDerivedClassDef();
			if (uniqueExprs.find(classDef) == uniqueExprs.end()) {
				uniqueExprs.insert(classDef);

				// and then generate the definition for this expression
				expr->appendDefinition(MaterialExpression::SHADER_HLSL, defs);
			}

			// special case -- samplers aren't regular constants so
			// track the samplers that this shader uses, so that we can 
			// look them up by name later if needed
			if (expr->getDerivedClassDef() == Sampler2DExpression::getClassDef()) {
				Sampler2DExpression* sampExp = static_cast<Sampler2DExpression*>(expr);
				mSamplers[sampExp->samplerName()] = sampExp;
			}
		}

		std::string defStr(defs.str());
		mSource.append(defStr.c_str());

		// declare the input struct (matches the VS output struct in VertexShaderD3D9)
		mSource.append("struct PSIN \n{\n    float4 HPOS : POSITION;\n    float4 COLOR : COLOR0;\n    float4 NORM : TEXCOORD0;\n    float4 TXC0 : TEXCOORD1;\n    float4 TXC2 : TEXCOORD2;\n};\n");

		// open the fragment shader...
		mSource.append("float4 PS(PSIN psin) : COLOR {\n");

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
		for (int i=0; i<exprCount; ++i) {
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
			for (size_t j=0; j<inputAttrs.size(); ++j) {
				// find this attribute's name in the connection map
				const Attribute* attr = &inputAttrs[j];
				AttrToVarName::iterator name = names.find(attr);

				std::string tmp("float(0)");
				std::string* arg = &tmp;

				if(name != names.end()) {
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

	return true;
}

bool FragmentShader::destroy()
{
	// release pixel shader, if initialized
	if (mPS) {
		mPS->Release();
		mPS = 0;
	}

	return true;
}

void FragmentShader::apply()
{
	assert(mDevice);

	// check to see if PS needs initialized
	if (!mPS) {
		initialize();
	}

	if (mDevice) {
		mDevice->SetPixelShader(mPS);
	}

	// set any shader constants we have
	if (mConstantTable) {
		D3DXCONSTANTTABLE_DESC desc;
		if (SUCCEEDED(mConstantTable->GetDesc(&desc))) {
			// set each constant we found during compilation
			for (UINT i=0; i<desc.Constants; ++i) {
				D3DXHANDLE pConst = mConstantTable->GetConstant(NULL, i);
				if (pConst) {
					D3DXCONSTANT_DESC constDesc;
					UINT ct = 1;
					mConstantTable->GetConstantDesc(pConst, &constDesc, &ct);
					switch (constDesc.Type) {
					case D3DXPT_SAMPLER2D:
						{
							Samplers::iterator it = mSamplers.find(constDesc.Name);
							if (it != mSamplers.end()) {
								Sampler2DExpression* sampExp = it->second;
								if (sampExp) {
									Texture2D* tex = static_cast<Texture2D*>(sampExp->getSampler2D().texture());
									mDevice->SetTexture(constDesc.RegisterIndex, tex->textureObject());
								}
							}
						}
						break;
					}
				}
			}
		}
	}
}

const char* FragmentShader::HLSL_COMMON =
"// This shader is autogenerated\n"
"\n"
"typedef float4 RGBA;\n"
"typedef float3 RGB;\n"
"\n"
;

} // Direct3D9
} // Gfx
} // Teardrop
