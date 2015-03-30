/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROGRAMOPENGL_INCLUDED)
#define PROGRAMOPENGL_INCLUDED

#include "GLHeaders.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace OpenGL
		{
			class FragmentShader;
			class VertexShader;

			class Program
			{
			public:
				Program();
				~Program();

				bool initialize(VertexShader* aVS, FragmentShader* aFS);
				bool destroy();
				void apply();
				void disable();

				TD_DECLARE_ALLOCATOR();

			protected:
				GLuint mProgramName = 0;
				VertexShader* mVS;
				FragmentShader* mFS;
			};
		}
	}
}

#endif // PROGRAMOPENGL_INCLUDED
