/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXDECLARATIOND3D9_INCLUDED)
#define VERTEXDECLARATIOND3D9_INCLUDED

#include "Gfx/VertexDeclaration.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Submesh;

		namespace Direct3D9 
		{
			class VertexDeclaration : public Gfx::VertexDeclaration
			{
			public:
				VertexDeclaration(IDirect3DDevice9* device, Submesh* parent);
				~VertexDeclaration();

				void rebuild();

				IDirect3DVertexDeclaration9* declaration();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;
				IDirect3DVertexDeclaration9* mDecl;
				D3DVERTEXELEMENT9* mElements;
			};
		}
	}
}

#endif // VERTEXDECLARATIOND3D9_INCLUDED
