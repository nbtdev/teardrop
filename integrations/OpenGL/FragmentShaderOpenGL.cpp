/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FragmentShaderOpenGL.h"
#include "Texture2DOpenGL.h"
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
namespace OpenGL {

FragmentShader::FragmentShader(ShaderConstantTable* constants, Material* mtl)
	: Gfx::FragmentShader(mtl)
	, mShaderName(0)
	, mConstants(constants)
{
}

FragmentShader::~FragmentShader()
{
	if (mShaderName)
		glDeleteShader(mShaderName);
}

bool FragmentShader::initialize()
{
	// build and compile pixel shader
	if (!mSource.length()) {
		// generate source from material definition/expressions

		// first the common parts
		mSource.append(GLSL_COMMON);

		// perform a topological sort on the shader expressions, so that we can order 
		// their invocation in order, as well as collect their definitions
		mMaterial->sortExpressions();

		// then functions for each expression in the shader; we can iterate the sorted
		// expressions and put out definitions for each unique expression ClassDef
		std::set<const Reflection::ClassDef*> uniqueExprs;
		int exprCount = mMaterial->expressionCount();
		MaterialExpression** expressions = mMaterial->sortedExpressions();

		std::stringstream defs;
		for (int i = 0; i<exprCount; ++i) {
			MaterialExpression* expr = expressions[i];
			const Reflection::ClassDef* classDef = expr->getDerivedClassDef();
			if (uniqueExprs.find(classDef) == uniqueExprs.end()) {
				uniqueExprs.insert(classDef);

				// and then generate the definition for this expression
				expr->appendDefinition(MaterialExpression::SHADER_GLSL4, defs);
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

		// declare the varying inputs
		// TODO: get this information from ShaderFeatures (somehow...)
		mSource.append("varying vec4 POSITION_HPOS;\nvarying vec4 COLOR0_COLOR;\nvarying vec4 TEXCOORD0_NORM;\nvarying vec4 TEXCOORD1_TXC0;\nvarying vec4 TEXCOORD2_TXC1;\n\n");

		// open the fragment shader...
		mSource.append("void main()\n{\n");

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
		mSource.append("\n    gl_FragColor = output;\n}\n");
		//mSource.append("\nreturn float4(0.5,0.5,0.5,1);\n}\n");
	}

	if (mSource.length() && !mShaderName) {
		// compile the shader
#if 0
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
#endif
	}

	return true;
}

bool FragmentShader::destroy()
{
	// release pixel shader, if initialized
	if (mShaderName) {
		glDeleteShader(mShaderName);
		mShaderName = 0;
	}

	return true;
}

void FragmentShader::apply()
{
#if 0
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
			for (UINT i = 0; i<desc.Constants; ++i) {
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
#endif 
}

const char* FragmentShader::GLSL_COMMON =
	"// This shader is autogenerated\n"
	"\n"
	"#define RGBA vec4;\n"
	"#define RGB vec3;\n"
	"#define float2 vec2;\n"
	"#define float3 vec3;\n"
	"#define float4 vec4;\n"
	"\n"
	;

} // OpenGL
} // Gfx
} // Teardrop
