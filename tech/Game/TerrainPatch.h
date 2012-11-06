/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDTERRAINPATCH_INCLUDED)
#define TDTERRAINPATCH_INCLUDED

#include "Game/ZoneObject.h"
#include "Gfx/GfxMeshInstance.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class Stream;
	class TerrainZone;
	struct TerrainVertex;
	struct TerrainVertex2;
	struct TerrainNormal;
	struct Environment;
	struct FourCC;
	class GfxBitmap;

	class TerrainPatch : 
		public ZoneObject
	{
		HResource m_hMesh;
		TerrainVertex* m_pVertexBuffer;
		TerrainNormal* m_pNormalBuffer;
		short* m_pIndexBuffer;
		TerrainZone* m_pOwner;

	public:
		static const FourCC& FOURCC;

		TD_CLASS(TerrainPatch, ZoneObject);

		TerrainPatch();
		~TerrainPatch();

		bool initialize();
		bool destroy();

		//! create patch from source heightmap data
		bool create(
			TerrainZone* pOwner, 
			const GfxBitmap& heightmap,
			size_t x, 
			size_t y);

		// we don't want these serialized since they are dynamically created
		bool shouldSerialize() const { return false; }

		void notifyPropertyChangedLocal(const Reflection::PropertyDef*);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // TDTERRAINPATCH_INCLUDED
