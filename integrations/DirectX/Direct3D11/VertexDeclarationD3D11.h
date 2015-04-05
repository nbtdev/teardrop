/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXDECLARATIOND3D11_INCLUDED)
#define VERTEXDECLARATIOND3D11_INCLUDED

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
				VertexDeclaration(ID3D11Device* device, Submesh* parent);
				~VertexDeclaration();

				void rebuild();

				ID3D11InputLayout* declaration();

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
