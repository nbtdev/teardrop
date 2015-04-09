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
				//IDirect3DPixelShader9* mPS;
				// the constants declared in the shader
				//LPD3DXCONSTANTTABLE mConstantTable;
				// the constants managed by the engine
				ShaderConstantTable* mConstants;

				struct ConstantBinding {
					int mCurrentVersion;
					ShaderConstant* mConstant;
				};

				typedef std::vector<ConstantBinding> ConstantBindings;
				ConstantBindings mBindings;

				struct SamplerEnt {
					SamplerEnt() {}
					SamplerEnt(Sampler2DExpression* aExpr, int aIndex) : mSamplerExpr(aExpr), mSamplerIndex(aIndex) {}
					Sampler2DExpression* mSamplerExpr = nullptr;
					int mSamplerIndex = 0;
				};
				typedef std::map<String, SamplerEnt> Samplers;
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
