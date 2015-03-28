/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADERMANAGEROPENGL_INCLUDED)
#define SHADERMANAGEROPENGL_INCLUDED

#include "Gfx/ShaderManager.h"
#include "Gfx/ShaderFeatures.h"
#include "Util/UUID.h"
#include <map>

namespace Teardrop
{
	namespace Gfx
	{
		namespace OpenGL
		{
			class FragmentShader;
			class VertexShader;

			class ShaderManager : public Gfx::ShaderManager
			{
			public:
				ShaderManager();
				~ShaderManager();

				Gfx::FragmentShader* createOrFindInstanceOf(Material* sourceMaterial);
				Gfx::VertexShader* createOrFindInstanceOf(Submesh* submesh);
				void release(Gfx::FragmentShader* shader);
				void release(Gfx::VertexShader* shader);

				TD_DECLARE_ALLOCATOR();

			protected:
				typedef std::map<UUID, OpenGL::FragmentShader*> FragmentShaders;
				FragmentShaders mFragmentShaders;

				typedef std::map<unsigned int, OpenGL::VertexShader*> VertexShaders;
				VertexShaders mVertexShaders;
			};
		}
	}
}

#endif // SHADERMANAGEROPENGL_INCLUDED
