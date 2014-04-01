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

namespace Teardrop
{
	namespace Gfx 
	{
		namespace Direct3D9
		{
			class FragmentShader : public Gfx::FragmentShader
			{
			public:
				FragmentShader(IDirect3DDevice9* device, Material* mtl);
				~FragmentShader();

				bool initialize();
				bool destroy();
				void apply();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;
				IDirect3DPixelShader9* mPS;

				String mSource; // in case we need to recompile the shader
				String mFullpath; // in case someone wants to save the shader?
				String mErrs; // in case we need to recompile the shader

				static const char* HLSL_COMMON;
			};
		}
	}
}

#endif // FRAGMENTSHADERD3D9_INCLUDED