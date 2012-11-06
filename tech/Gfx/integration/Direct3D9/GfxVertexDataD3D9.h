/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXVERTEXDATAD3D9_INCLUDED)
#define GFXVERTEXDATAD3D9_INCLUDED

#include "GfxVertexData.h"

namespace Teardrop
{
	struct Environment; 

	class GfxVertexDataD3D9 : public GfxVertexData
	{
		DECLARE_SERIALIZABLE(GfxVertexDataD3D9);

		SerialPointer<IDirect3DVertexBuffer9> m_pD3D9VertexBuffer;

	public:
		GfxVertexDataD3D9();
		GfxVertexDataD3D9(int);
		~GfxVertexDataD3D9();

		bool initialize(
			CreationFlags flags=STATIC, 
			unsigned int vertSize=0, 
			unsigned int vertCount=0, 
			void* pData=0);

		bool destroy();
		bool release();
		void releaseD3DResources();
		void* lock(bool bDiscardExisting=true);
		void unlock();
		IDirect3DVertexBuffer9* getD3D9VertexBuffer();
		bool delayInit();

		bool serialize(ResourceSerializer& serializer);

		DECLARE_GFX_ALLOCATOR();

	public:
		bool initD3D9();
	};
}

#endif // GFXVERTEXDATAD3D9_INCLUDED
