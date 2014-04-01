/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FRAGMENTSHADER_INCLUDED)
#define FRAGMENTSHADER_INCLUDED

#include "Gfx/ShaderFeatures.h"
#include "Memory/Allocators.h"
#include "Util/UUID.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Material;

		class FragmentShader
		{
		public:
			FragmentShader(Material* material);
			~FragmentShader();

			virtual bool initialize();
			virtual bool destroy();
			virtual void apply() = 0;

			UUID materialId();
			const ShaderFeatures& features();

			TD_DECLARE_ALLOCATOR();

		protected:
			Material* mMaterial;
			ShaderFeatures mFeatures;
		};
	}
}

#endif // FRAGMENTSHADER_INCLUDED
