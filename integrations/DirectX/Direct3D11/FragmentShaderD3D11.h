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

#if !defined(FRAGMENTSHADERD3D11_INCLUDED)
#define FRAGMENTSHADERD3D11_INCLUDED

#include "Gfx/FragmentShader.h"
#include "Util/_String.h"
#include <map>
#include <memory>
#include <vector>

namespace Teardrop
{
	namespace Gfx
	{
		class ShaderConstantTable;
		class ShaderConstant;
		class Sampler2DExpression;
		class Texture2D;

		namespace Direct3D11
		{
			class FSEnvironment;

			class FragmentShader : public Gfx::FragmentShader
			{
			public:
				FragmentShader(ComPtr<ID3D11Device> aDevice, ShaderConstantTable* constants, Material* mtl);
				~FragmentShader();

				void apply();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;
				ComPtr<ID3DBlob> mBytecode;
				ComPtr<ID3D11PixelShader> mPS;
				ComPtr<ID3D11ClassLinkage> mLinkage;
				ShaderConstantTable* mConstants;

				struct ConstantBinding {
					int mCurrentVersion;
					ShaderConstant* mConstant;
				};

				typedef std::vector<ConstantBinding> ConstantBindings;
				ConstantBindings mBindings;

				typedef std::vector<Sampler2DExpression*> Sampler2DExpressions;
				Sampler2DExpressions mSamplerExpressions;

				typedef std::vector<ComPtr<ID3D11SamplerState>> Samplers;
				Samplers mSamplers;

				std::unique_ptr<FSEnvironment> mEnvironment;

				String mSource; // in case we need to recompile the shader
				String mFullpath; // in case someone wants to save the shader?
				String mErrs; // in case we need to recompile the shader

				static const char* HLSL_COMMON;
			};
		}
	}
}

#endif // FRAGMENTSHADERD3D11_INCLUDED
