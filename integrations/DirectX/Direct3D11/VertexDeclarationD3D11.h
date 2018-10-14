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

#if !defined(VERTEXDECLARATIOND3D11_INCLUDED)
#define VERTEXDECLARATIOND3D11_INCLUDED

#include "stdafx.h"
#include "Gfx/VertexDeclaration.h"

namespace Teardrop
{
	namespace Gfx
	{
		class Submesh;

		namespace Direct3D11
		{
			class VertexDeclaration : public Gfx::VertexDeclaration
			{
			public:
				VertexDeclaration(ComPtr<ID3D11Device> device, Submesh* parent);
				~VertexDeclaration();

				void rebuild();

				ID3D11InputLayout* layout();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;
				ComPtr<ID3D11InputLayout> mLayout;
				D3D11_INPUT_ELEMENT_DESC* mElements = nullptr;
			};
		}
	}
}

#endif // VERTEXDECLARATIOND3D11_INCLUDED
