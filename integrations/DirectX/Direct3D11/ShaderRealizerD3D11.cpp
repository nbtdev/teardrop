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

#include "ShaderRealizerD3D11.h"
#include "Stream\Stream.h"

namespace {
	const char* sTypes[] = {
		"void",            // TYPE_VOID,
		"float",           // TYPE_FLOAT,
		"float2",          // TYPE_FLOAT2,
		"float3",          // TYPE_FLOAT3,
		"float4",          // TYPE_FLOAT4,
		"texture2D",       // TYPE_SAMPLER,
		"void",            // TYPE_UNKNOWN,
	};

	const char* HLSL_COMMON =
		"// This shader is autogenerated\n"
		"\n"
		"typedef float4 RGBA;\n"
		"typedef float3 RGB;\n"
		"\n"
		;
} // namespace 

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

ShaderRealizer::ShaderRealizer() 
{

}

ShaderRealizer::~ShaderRealizer()
{

}

void ShaderRealizer::startingRealize(Stream& aStream) const
{
	aStream.write(HLSL_COMMON, strlen(HLSL_COMMON));
}

const char* ShaderRealizer::lookupTypeString(Codegen::DataType aType) const
{
	return sTypes[aType];
}

} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
