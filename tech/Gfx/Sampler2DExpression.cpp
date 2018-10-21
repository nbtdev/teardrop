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

void Sampler2DExpression::appendDefinition(Language lang, size_t aTextureIndex, size_t aSampIndex, std::ostream& o)
{
	// call our "custom" insertDependencies
	insertDependencies(lang, aTextureIndex, aSampIndex, o);

	// and then the "real" appendDefinition
	MaterialExpression::appendDefinition(lang, o);
}

void Sampler2DExpression::insertDependencies(Language lang, size_t aTextureIndex, size_t aSampIndex, std::ostream& o)
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
