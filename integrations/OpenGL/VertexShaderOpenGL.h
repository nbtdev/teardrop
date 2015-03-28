/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXSHADEROPENGL_INCLUDED)
#define VERTEXSHADEROPENGL_INCLUDED

#include "Gfx/VertexShader.h"
#include "Util/_String.h"
#include "GLHeaders.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx
	{
		class ShaderConstant;
		class ShaderConstantTable;

		namespace OpenGL
		{
			class VertexShader : public Gfx::VertexShader
			{
			public:
				VertexShader(ShaderConstantTable* constants);
				~VertexShader();

				bool initialize(Submesh* submesh);
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

				String mSource; // in case we need to recompile the shader
				String mFullpath; // in case someone wants to save the shader?
				String mErrs; // in case we need to recompile the shader
			};
		}
	}
}

#endif // VERTEXSHADEROPENGL_INCLUDED
