/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BUFFERMANAGERD3D11_INCLUDED)
#define BUFFERMANAGERD3D11_INCLUDED

#include "Gfx/BufferManager.h"
#include <list>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class Renderer;
			class IndexBuffer;
			class VertexBuffer;
			class VertexDeclaration;

			class BufferManager : public Gfx::BufferManager
			{
			public:
				BufferManager(Renderer* aRenderer);
				~BufferManager();

				Gfx::IndexBuffer* createIndexBuffer(Submesh* parent);
				Gfx::VertexBuffer* createVertexBuffer(Submesh* parent);
				Gfx::VertexDeclaration* createVertexDeclaration(Submesh* parent);
				void release(Gfx::IndexBuffer* buffer);
				void release(Gfx::VertexBuffer* buffer);
				void release(Gfx::VertexDeclaration* decl);

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;

				typedef std::list<IndexBuffer*> IndexBuffers;
				typedef std::list<VertexBuffer*> VertexBuffers;
				typedef std::list<VertexDeclaration*> VertexDeclarations;
				IndexBuffers mIndexBuffers;
				VertexBuffers mVertexBuffers;
				VertexDeclarations mVertexDeclarations;
			};
		}
	}
}

#endif // BUFFERMANAGERD3D11_INCLUDED
