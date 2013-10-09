/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADER_INCLUDED)
#define SHADER_INCLUDED

#include "Util/UUID.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Material;

		class Shader
		{
		public:
			Shader(Material* material);
			~Shader();

			virtual bool initialize();
			virtual bool destroy();

			UUID materialId();

			TD_DECLARE_ALLOCATOR();

		protected:
			Material* mMaterial;
		};
	}
}

#endif // SHADER_INCLUDED
