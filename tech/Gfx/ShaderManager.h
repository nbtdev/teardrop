/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADERMANAGER_INCLUDED)
#define SHADERMANAGER_INCLUDED

#include "Memory/Allocators.h"
#include "Core/Singleton.h"

namespace Teardrop
{
	namespace Gfx
	{
		class FragmentShader;
		class VertexShader;
		class Material;
		class Submesh;
		class ShaderConstantTable;

		class ShaderManager : public Singleton<ShaderManager>
		{
		public:
			ShaderManager();
			~ShaderManager();

			virtual FragmentShader* createOrFindInstanceOf(Material* sourceMaterial) = 0;
			virtual VertexShader* createOrFindInstanceOf(Submesh* submesh) = 0;
			virtual void release(FragmentShader* shader) = 0;
			virtual void release(VertexShader* shader) = 0;

			ShaderConstantTable* constantTable();

			TD_DECLARE_ALLOCATOR();

		protected:
			ShaderConstantTable* mConstants;
		};
	}
}

#endif // SHADERMANAGER_INCLUDED
