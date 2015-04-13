/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
