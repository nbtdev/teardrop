/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MATERIAL_INCLUDED)
#define MATERIAL_INCLUDED

#include "Reflection/Reflection.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Sampler2D;
		class Shader;

		class Material : public Reflection::Object
		{
		public:
			TD_CLASS(Material, Object);
			TD_CLASS_CREATABLE();

			Material();
			~Material();

			bool initialize();
			bool destroy();

			Sampler2D* samplers2D();
			int numSamplers2D();

			Shader* shader();

			TD_DECLARE_ALLOCATOR();

		protected:
			Sampler2D* mSamplers2D;
			int mNumSamplers2D;

			// will hold pointer to platform/renderer-specific shader instance of this material
			Shader* mShader;
		};
	}
}

#endif // MATERIAL_INCLUDED
