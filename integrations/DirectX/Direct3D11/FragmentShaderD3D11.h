/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
