/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXSHADERD3D11_INCLUDED)
#define VERTEXSHADERD3D11_INCLUDED

#include "Gfx/VertexShader.h"
#include "Util/_String.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx
	{
		class ShaderConstant;
		class ShaderConstantTable;

		namespace Direct3D11
		{
			class VertexShader : public Gfx::VertexShader
			{
			public:
				VertexShader(ComPtr<ID3D11Device> aDevice, ShaderConstantTable* constants, Submesh* submesh);
				~VertexShader();

				void apply();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;
				//IDirect3DVertexShader9* mVS;
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

#endif // VERTEXSHADERD3D11_INCLUDED
