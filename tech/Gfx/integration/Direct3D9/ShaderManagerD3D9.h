/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADERMANAGERD3D9_INCLUDED)
#define SHADERMANAGERD3D9_INCLUDED

#include "Gfx/ShaderManager.h"
#include "Util/UUID.h"
#include <map>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D9 
		{
			class Shader;

			class ShaderManager : public Gfx::ShaderManager
			{
			public:
				ShaderManager(IDirect3DDevice9* device);
				~ShaderManager();

				Gfx::Shader* createOrFindInstanceOf(Material* sourceMaterial);
				void release(Gfx::Shader* shader);

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;

				typedef std::map<UUID, Direct3D9::Shader*> Shaders;
				Shaders mShaders;
			};
		}
	}
}

#endif // SHADERMANAGERD3D9_INCLUDED
