/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADERD3D9_INCLUDED)
#define SHADERD3D9_INCLUDED

#include "Gfx/Shader.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Gfx 
	{
		namespace Direct3D9
		{
			class Shader : public Gfx::Shader
			{
			public:
				Shader(IDirect3DDevice9* device, Material* mtl);
				~Shader();

				bool initialize();
				bool destroy();
				void apply();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;
				ID3DXEffect* mFX;
				D3DXHANDLE mTechnique;
				D3DXHANDLE mTechniqueShadowCast;
				D3DXHANDLE mTechniqueShadowRecv;
				String mSource; // in case we need to recompile the shader
				String mFullpath; // in case someone wants to save the shader?
				String mErrs; // in case we need to recompile the shader			
			};
		}
	}
}

#endif // SHADERD3D9_INCLUDED
