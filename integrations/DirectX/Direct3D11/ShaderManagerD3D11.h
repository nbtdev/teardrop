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

#if !defined(SHADERMANAGERD3D11_INCLUDED)
#define SHADERMANAGERD3D11_INCLUDED

#include "Gfx/ShaderManager.h"
#include "Gfx/ShaderFeatures.h"
#include "Util/UUID.h"
#include <map>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class FragmentShader;
			class VertexShader;

			class ShaderManager : public Gfx::ShaderManager
			{
			public:
				ShaderManager(ComPtr<ID3D11Device> aDevice);
				~ShaderManager();

				Gfx::FragmentShader* createOrFindInstanceOf(Material* sourceMaterial);
				Gfx::VertexShader* createOrFindInstanceOf(Submesh* submesh);
				void release(Gfx::FragmentShader* shader);
				void release(Gfx::VertexShader* shader);

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;

				typedef std::map<UUID, FragmentShader*> FragmentShaders;
				FragmentShaders mFragmentShaders;

				typedef std::map<unsigned int, VertexShader*> VertexShaders;
				VertexShaders mVertexShaders;
			};
		}
	}
}

#endif // SHADERMANAGERD3D11_INCLUDED
