/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXSHADERD3D9_INCLUDED)
#define VERTEXSHADERD3D9_INCLUDED

#include "Gfx/VertexShader.h"
#include "Util/_String.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class ShaderConstant;

		namespace Direct3D9
		{
			class VertexShader : public Gfx::VertexShader
			{
			public:
				VertexShader(IDirect3DDevice9* device);
				~VertexShader();

				bool initialize(Submesh* submesh);
				bool destroy();
				void apply();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;
				IDirect3DVertexShader9* mVS;
				LPD3DXCONSTANTTABLE mConstantTable;
				typedef std::vector<ShaderConstant*> ConstantBindings;
				ConstantBindings mBindings;

				String mSource; // in case we need to recompile the shader
				String mFullpath; // in case someone wants to save the shader?
				String mErrs; // in case we need to recompile the shader
			};
		}
	}
}

#endif // VERTEXSHADERD3D9_INCLUDED