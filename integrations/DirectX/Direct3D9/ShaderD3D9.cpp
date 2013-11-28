/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ShaderD3D9.h"
#include "Gfx/Material.h"
#include "Gfx/Connection.h"
#include "Gfx/Attribute.h"
#include "Gfx/MaterialExpression.h"
#include <sstream>
#include <set>
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

Shader::Shader(IDirect3DDevice9* device, Material* mtl)
	: Gfx::Shader(mtl)
	, mDevice(device)
	, mVS(0)
	, mPS(0)
	, mFX(0)
	, mTechnique(0)
	, mTechniqueShadowCast(0)
	, mTechniqueShadowRecv(0)
{
	assert(mDevice);
}

Shader::~Shader()
{
}

bool Shader::initialize()
{
	// build and compile vertex and pixel shaders
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

		// then generate the function calls; in order to manage input and output params/vars, we need 
		// to associate the ends of the connections with variable names (mainly, the input, or "from"
		// end of a connection would contain the output of the "from" attribute, which is the input for
		// the "to" attribute/expression)
		struct Temp { 
			const Attribute* mInput; 
			std::string mName;
			Temp() : mInput(0) {}
			Temp(const Temp& other) { *this = other; }
			Temp& operator=(const Temp& other) { mInput = other.mInput; mName = other.mName; return *this; }
		};

		typedef std::map<const Attribute*, Temp> ConnectionToVarName;
		ConnectionToVarName names;

		// go through all connections and form this map
		int nConnections = mMaterial->connections(0, 0);
		std::vector<Connection*> connections(nConnections);
		mMaterial->connections(&connections[0], nConnections);

		for (int i=0; i<nConnections; ++i) {
			Temp temp;
			temp.mInput = connections[i]->input();
			names[connections[i]->output()] = temp;
		}

		// of course this also means we need to iterate the expressions again...
		std::stringstream calls;
		for (int i=0; i<exprCount; ++i) {
			MaterialExpression* expr = expressions[i];

			// our input attributes are someone else's output attributes, so collect their names
			// into a std::vector of strings
			const MaterialExpression::Attributes& inputAttrs = expr->inputAttributes();
			std::vector<std::string> inputs(inputAttrs.size());

			int nameIdx = 0;
			for (size_t j=0; j<inputAttrs.size(); ++i) {
				// find this attribute's name in the connection map
				const Attribute* addr = &inputAttrs[j];
				ConnectionToVarName::iterator name = names.find(addr);

				// this should be found
				assert(name != names.end());

				if (name != names.end()) {
					inputs[nameIdx++] = name->second.mName;
				}
			}

			// then we can generate the code for this call
			std::vector<MaterialExpression::OutputName> outputNames;
			expr->appendCall(
				MaterialExpression::SHADER_HLSL,
				i,
				inputs,
				outputNames,
				calls
				);

			// and finally, we can put the new output names in the attr-to-name map for later use
			for (size_t j=0; j<outputNames.size(); ++j) {
				ConnectionToVarName::iterator attrIt = names.find(outputNames[j].mOutputAttr);

				// this should also be found
				assert(attrIt != names.end());

				if (attrIt != names.end()) {
					attrIt->second.mName = outputNames[j].mOutputVarName;
				}
			}
		}

		std::string callStr(calls.str());
		mSource.append(callStr.c_str());
	}

	return true;
}

bool Shader::destroy()
{
	// release vertex and pixel shaders, if initialized
	if (mVS) {
		mVS->Release();
		mVS = 0;
	}

	if (mPS) {
		mPS->Release();
		mPS = 0;
	}

	return true;
}

void Shader::apply()
{
	assert(mDevice);

	// check to see if VS/PS need initialized
	if (!mVS && !mPS) {
		initialize();
	}
}

const char* Shader::HLSL_COMMON =
"// This shader is autogenerated\n"
"\n"
"typedef float4 RGBA;\n"
"typedef float3 RGB;\n"
"\n"
;

} // Direct3D9
} // Gfx
} // Teardrop
