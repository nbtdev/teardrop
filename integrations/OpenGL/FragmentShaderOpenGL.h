/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FRAGMENTSHADEROPENGL_INCLUDED)
#define FRAGMENTSHADEROPENGL_INCLUDED

#include "GLHeaders.h"
#include "Gfx/FragmentShader.h"
#include "Util/_String.h"
#include <map>
#include <vector>

namespace Teardrop
{
	namespace Gfx
	{
		class ShaderConstantTable;
		class ShaderConstant;
		class Sampler2DExpression;

		namespace OpenGL
		{
			class FragmentShader : public Gfx::FragmentShader
			{
			public:
				FragmentShader(ShaderConstantTable* constants, Material* mtl);
				~FragmentShader();

				bool initialize();
				bool destroy();
				void apply();

				TD_DECLARE_ALLOCATOR();

			protected:
				GLuint mShaderName;
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

				typedef std::map<String, Sampler2DExpression*> Samplers;
				Samplers mSamplers;

				String mSource; // in case we need to recompile the shader
				String mFullpath; // in case someone wants to save the shader?
				String mErrs; // in case we need to recompile the shader

				static const char* GLSL_COMMON;
			};
		}
	}
}

#endif // FRAGMENTSHADEROPENGL_INCLUDED
