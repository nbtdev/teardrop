/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADERMANAGERD3D11_INCLUDED)
#define SHADERMANAGERD3D11_INCLUDED

#include "Gfx/ShaderManager.h"
#include "Gfx/ShaderFeatures.h"
#include "Util/UUID.h"
#include <map>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class FragmentShader;
			class VertexShader;

			class ShaderManager : public Gfx::ShaderManager
			{
			public:
				ShaderManager(ComPtr<ID3D11Device> aDevice);
				~ShaderManager();

				Gfx::FragmentShader* createOrFindInstanceOf(Material* sourceMaterial);
				Gfx::VertexShader* createOrFindInstanceOf(Submesh* submesh);
				void release(Gfx::FragmentShader* shader);
				void release(Gfx::VertexShader* shader);

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;

				typedef std::map<UUID, FragmentShader*> FragmentShaders;
				FragmentShaders mFragmentShaders;

				typedef std::map<unsigned int, VertexShader*> VertexShaders;
				VertexShaders mVertexShaders;
			};
		}
	}
}

#endif // SHADERMANAGERD3D11_INCLUDED
