/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXINDEXDATAD3D9_INCLUDED)
#define GFXINDEXDATAD3D9_INCLUDED

#include "GfxIndexData.h"
#include "GfxCommon.h"
#include "GfxRenderer.h"
#include "GfxRendererD3D9.h"
#include "Util/Environment.h"
#include <assert.h>
#include <new.h>

namespace Teardrop
{
	class GfxIndexDataD3D9 : public GfxIndexData
	{
		DECLARE_SERIALIZABLE(GfxIndexDataD3D9);

		SerialPointer<IDirect3DIndexBuffer9> m_pD3D9IndexBuffer;

	public:
		GfxIndexDataD3D9();
		GfxIndexDataD3D9(int);
		~GfxIndexDataD3D9();

		bool initialize(
			unsigned int indexSize = 0,
			unsigned int indexCount = 0,
			void *pData = 0);
		bool destroy();
		bool release();
		void releaseD3DResources();
		bool delayInit();

		void* lock(bool bDiscardExisting=true);
		void unlock();

		IDirect3DIndexBuffer9* getD3D9IndexBuffer() { return m_pD3D9IndexBuffer; }

		bool serialize(ResourceSerializer& serializer);

		DECLARE_GFX_ALLOCATOR();

	private:
		bool initD3D9();
	};
}
#endif // GFXINDEXDATAD3D9_INCLUDED
