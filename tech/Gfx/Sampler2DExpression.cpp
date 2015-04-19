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


#include "Sampler2DExpression.h"
#include "Sampler2D.h"
#include "Codegen/Argument.h"
#include "Codegen/AttributeExpression.h"
#include "Codegen/BinaryExpression.h"
#include "Codegen/CastExpression.h"
#include "Codegen/Common.h"
#include "Codegen/Function.h"
#include "Codegen/SamplerExpression.h"
#include "Util/StringUtil.h"
#include <ostream>

namespace Teardrop {
namespace Gfx {

using Codegen::Argument;
using Codegen::AttributeExpression;
using Codegen::BinaryExpression;
using Codegen::CastExpression;
using Codegen::Expression;
using Codegen::SamplerExpression;

TD_CLASS_IMPL(Sampler2DExpression);

Sampler2DExpression::Sampler2DExpression()
{
}

Sampler2DExpression::~Sampler2DExpression()
{
}

bool Sampler2DExpression::initialize()
{
	mFunction.reset(TD_NEW Codegen::Function("Sampler2DExpression"));

	Attribute attr1("Texcoord", ATTR_FLOAT2, this, Attribute::Optional, "psin.TEXCOORD0");
	Attribute attr2("Color", ATTR_RGBA, this);
	Attribute attr3("Color3", ATTR_RGB, this);

	mInputs.push_back(attr1);
	mFeatures.setFeature(INTERP_TEXCOORD, 0);
	mOutputs.push_back(attr2);
	mOutputs.push_back(attr3);

	// add arguments
	AttributeExpression::Ptr attrExpr1(TD_NEW AttributeExpression(attr1));
	AttributeExpression::Ptr attrExpr2(TD_NEW AttributeExpression(attr2));
	AttributeExpression::Ptr attrExpr3(TD_NEW AttributeExpression(attr3));

	Argument::Ptr arg1(TD_NEW Argument(Codegen::DIRECTION_IN, attrExpr1));
	Argument::Ptr arg2(TD_NEW Argument(Codegen::DIRECTION_OUT, attrExpr2));
	Argument::Ptr arg3(TD_NEW Argument(Codegen::DIRECTION_OUT, attrExpr3));

	Expression::Ptr samp(TD_NEW SamplerExpression("samp", &getSampler2D(), attrExpr1));

	Argument::Ptr argSamp(TD_NEW Argument("samp", Codegen::DIRECTION_IN, samp));

	mFunction->addArgument(argSamp);
	mFunction->addArgument(arg1);
	mFunction->addArgument(arg2);
	mFunction->addArgument(arg3);

	/*
		void Sampler2DExpression(in <texture> samp, in float2 Texcoord, out RGBA Color, out RGB Color3)
		{
		    Color = <tex2D>(samp, Texcoord);
		    Color3 = float3(Color); // or possibly "Color3 = Color.rgb;"
		}
	*/

	mFunction->addStatement(
		Expression::Ptr(TD_NEW BinaryExpression(
			attrExpr2, 
			Codegen::BINARY_OP_ASSIGN, 
			samp
			)
		));
		
	mFunction->addStatement(
		Expression::Ptr(TD_NEW BinaryExpression(
			attrExpr3,
			Codegen::BINARY_OP_ASSIGN,
			Expression::Ptr(TD_NEW CastExpression(Codegen::TYPE_FLOAT3, attrExpr2))
			)
		));

	return true;
}

void Sampler2DExpression::appendBody(Language lang, std::ostream& o)
{
	switch (lang) {
	case SHADER_HLSL5:
		o << std::string("    Color = ") << mTextureName << ".Sample(" << mSamplerName << ", Texcoord);" << std::endl;
		o << std::string("    Color3 = float3(Color.r, Color.g, Color.b);") << std::endl;
		break;
	case SHADER_GLSL4:
		o << std::string("    Color = texture2D(");
		o << mTextureName;
		o << std::string(", Texcoord);") << std::endl;

		o << std::string("    Color3 = float3(Color.r, Color.g, Color.b);") << std::endl;
		break;
	default:
		break;
	}
}

void Sampler2DExpression::appendDefinition(Language lang, int aTextureIndex, int aSampIndex, std::ostream& o)
{
	// call our "custom" insertDependencies
	insertDependencies(lang, aTextureIndex, aSampIndex, o);

	// and then the "real" appendDefinition
	MaterialExpression::appendDefinition(lang, o);
}

void Sampler2DExpression::insertDependencies(Language lang, int aTextureIndex, int aSampIndex, std::ostream& o)
{
	Sampler2D& samp = getSampler2D();
	if (samp.texture()) {
		// name only needs to be unique long enough to generate the shader(s)
		if (mSamplerName.length() == 0) {
			String tmp;
			StringUtil::toString(aTextureIndex, tmp);
			mTextureName = "tex2D_";
			mTextureName.append(tmp);
			StringUtil::toString(aSampIndex, tmp);
			mSamplerName = "samplerState_";
			mSamplerName.append(tmp);
		}

		switch (lang) {
			// be tricky here...GLSL4 only needs "uniform" in front of the same declaration that HLSL uses,
			// so insert that then fall through to the remainder of the uniform decl
		case SHADER_GLSL4:
			o << "uniform sampler2D " << mTextureName << std::endl;
			break;

		case SHADER_HLSL5:
			o << "texture2D " << mTextureName << " : register(t" << aTextureIndex << ");" << std::endl;

			// TODO: for now, just one D3D11 sampler object for all textures in the material...we'll get 
			// more complicated later when necessary
			o << "SamplerState " << mSamplerName << " : register(s" << aSampIndex << ");" << std::endl;
			break;
		default:
			break;
		}
	}
}

const String& Sampler2DExpression::textureName()
{
	return mTextureName;
}

const String& Sampler2DExpression::samplerName()
{
	return mSamplerName;
}

void Sampler2DExpression::insertFunctionName(Language /*lang*/, std::ostream& o)
{
	const Reflection::ClassDef* classDef = getDerivedClassDef();
	o << classDef->getName();
	o << '_';
	o << (void*)this;
}

} // namespace Gfx
} // namespace Teardrop
