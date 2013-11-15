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
		class Shader;
		class Material;

		class ShaderManager : public Singleton<ShaderManager>
		{
		public:
			ShaderManager();
			~ShaderManager();

			virtual Shader* createOrFindInstanceOf(Material* sourceMaterial) = 0;
			virtual void release(Shader* shader) = 0;

			TD_DECLARE_ALLOCATOR();

		protected:
		};
	}
}

#endif // SHADERMANAGER_INCLUDED
