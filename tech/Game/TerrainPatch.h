/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDTERRAINPATCH_INCLUDED)
#define TDTERRAINPATCH_INCLUDED

#include "Game/ZoneObject.h"
#include "Gfx/IMeshInstanceProvider.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class TerrainZone;
	struct TerrainVertex;
	struct TerrainVertex2;
	struct TerrainNormal;
	struct Environment;

    namespace Gfx {
        class Bitmap;
    }

	class TerrainPatch : 
		public ZoneObject
	{
		TerrainVertex* m_pVertexBuffer;
		TerrainNormal* m_pNormalBuffer;
		short* m_pIndexBuffer;
		TerrainZone* m_pOwner;

	public:
		TD_CLASS(TerrainPatch, ZoneObject);

		TerrainPatch();
		~TerrainPatch();

		bool initialize();
		bool destroy();

		//! create patch from source heightmap data
		bool create(
			TerrainZone* pOwner, 
            const Gfx::Bitmap& heightmap,
			size_t x, 
			size_t y);

		// we don't want these serialized since they are dynamically created
		bool shouldSerialize() const { return false; }

		void notifyPropertyChangedLocal(const Reflection::PropertyDef*);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // TDTERRAINPATCH_INCLUDED
