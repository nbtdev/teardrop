/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "FragmentShaderD3D9.h"
#include "Gfx/Material.h"
#include "Gfx/Connection.h"
#include "Gfx/Attribute.h"
#include "Gfx/MaterialExpression.h"
#include "Gfx/MaterialOutput.h"
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
}

FragmentShader::~FragmentShader()
{
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
		}

		std::string defStr(defs.str());
		mSource.append(defStr.c_str());

		// The VertexShaderD3D9 class will have an output struct that may not match these fragment shader
		// inputs; whether or not each of these semantics exist depends on the person using the content tools.
		// If a mesh does not provide something that the fragment shader expects, results are undefined
		// (but usually visually noticeable) and it's up to the user to fix it in the art assets. 
		mSource.append("struct PSIN \n{\n    float4 HPOS : TEXCOORD0;\n    float4 NORM : TEXCOORD1;\n    float4 TXC0 : TEXCOORD2;\n    float4 TXC1 : TEXCOORD3;\n};\n");

		// open the fragment shader...
		mSource.append("float4 PS(PSIN psin) : COLOR {\n");

		// then generate the function calls
		typedef std::map<const Attribute*, std::string> AttrToVarName;
		AttrToVarName names;

		// go through all connections and form this map
		int nConnections = mMaterial->connections(0, 0);
		std::vector<Connection*> connections(nConnections);
		mMaterial->connections(&connections[0], nConnections);

		// variable names should be based on the name (type) of the expression
		// that owns the "from" (output) attr; for uniqueness, we'll just assign
		// monotonically-increasing ordinals to each connection
		int ord = 0;
		for (int i=0; i<nConnections; ++i) {
			std::stringstream ss;
			Gfx::Attribute* out = connections[i]->output();
			MaterialExpression* me = out->mParent;
			ss << me->getDerivedClassDef()->getName() << '_' << ord++ << '_' << out->mName;
			std::string tmp(ss.str());

			names[connections[i]->output()] = tmp;
			names[connections[i]->input()] = tmp;
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
	}

	if (mSource.length() && !mPS) {
		// compile the shader
		LPD3DXBUFFER pErrorMsgs;
		LPD3DXBUFFER pShader;
		LPD3DXCONSTANTTABLE pConstants;

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
			&pConstants // constant table
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

		if (pShader)
			pShader->Release();

		if (pConstants)
			pConstants->Release();
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
