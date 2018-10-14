/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
