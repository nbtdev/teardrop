/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FRAGMENTSHADERD3D9_INCLUDED)
#define FRAGMENTSHADERD3D9_INCLUDED

#include "Gfx/FragmentShader.h"
#include "Util/_String.h"
#include <map>

namespace Teardrop
{
	namespace Gfx 
	{
		class ShaderConstantTable;
		class ShaderConstant;
		class Sampler2DExpression;

		namespace Direct3D9
		{
			class FragmentShader : public Gfx::FragmentShader
			{
			public:
				FragmentShader(IDirect3DDevice9* device, ShaderConstantTable* constants, Material* mtl);
				~FragmentShader();

				bool initialize();
				bool destroy();
				void apply();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;
				IDirect3DPixelShader9* mPS;
				// the constants declared in the shader
				LPD3DXCONSTANTTABLE mConstantTable;
				// the constants managed by the engine
				ShaderConstantTable* mConstants;

				struct ConstantBinding {
					int mCurrentVersion;
					ShaderConstant* mConstant;
				};

				typedef std::vector<ConstantBinding> ConstantBindings;
				ConstantBindings mBindings;

				typedef std::map<String, Sampler2DExpression*> Samplers;
				Samplers mSamplers;

				String mSource; // in case we need to recompile the shader
				String mFullpath; // in case someone wants to save the shader?
				String mErrs; // in case we need to recompile the shader

				static const char* HLSL_COMMON;
			};
		}
	}
}

#endif // FRAGMENTSHADERD3D9_INCLUDED