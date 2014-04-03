/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADERMANAGERD3D9_INCLUDED)
#define SHADERMANAGERD3D9_INCLUDED

#include "Gfx/ShaderManager.h"
#include "Gfx/ShaderFeatures.h"
#include "Util/UUID.h"
#include <map>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D9 
		{
			class FragmentShader;
			class VertexShader;

			class ShaderManager : public Gfx::ShaderManager
			{
			public:
				ShaderManager(IDirect3DDevice9* device);
				~ShaderManager();

				Gfx::FragmentShader* createOrFindInstanceOf(Material* sourceMaterial);
				Gfx::VertexShader* createOrFindInstanceOf(Submesh* submesh);
				void release(Gfx::FragmentShader* shader);
				void release(Gfx::VertexShader* shader);

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;

				typedef std::map<UUID, Direct3D9::FragmentShader*> FragmentShaders;
				FragmentShaders mFragmentShaders;

				typedef std::map<unsigned int, Direct3D9::VertexShader*> VertexShaders;
				VertexShaders mVertexShaders;
			};
		}
	}
}

#endif // SHADERMANAGERD3D9_INCLUDED
