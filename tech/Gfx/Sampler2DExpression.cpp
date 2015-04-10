/***************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "Sampler2DExpression.h"
#include "Sampler2D.h"
#include "Util/StringUtil.h"
#include <ostream>

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(Sampler2DExpression);

Sampler2DExpression::Sampler2DExpression()
{
}

Sampler2DExpression::~Sampler2DExpression()
{
}

bool Sampler2DExpression::initialize()
{
	mInputs.push_back(Attribute("Texcoord", ATTR_FLOAT2, this, Attribute::Optional, "psin.TEXCOORD0"));
	mFeatures.setFeature(INTERP_TEXCOORD, 0);
	mOutputs.push_back(Attribute("Color", ATTR_RGBA, this));
	mOutputs.push_back(Attribute("Color3", ATTR_RGB, this));

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