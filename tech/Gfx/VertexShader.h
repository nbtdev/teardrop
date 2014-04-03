/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXSHADER_INCLUDED)
#define VERTEXSHADER_INCLUDED

#include "Gfx/ShaderFeatures.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Submesh;

		class VertexShader
		{
		public:
			VertexShader();
			~VertexShader();

			virtual bool initialize(Submesh* submesh);
			virtual bool destroy();
			virtual void apply() = 0;

			unsigned int hash();

			TD_DECLARE_ALLOCATOR();

		protected:
			unsigned int mHash;
		};
	}
}

#endif // VERTEXSHADER_INCLUDED
