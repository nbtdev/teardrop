/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(COSTERRAINPATCH_INCLUDED)
#define COSTERRAINPATCH_INCLUDED

#include "Game/include/ZoneObject.h"
#include "Gfx/include/GfxMeshInstance.h"
#include "Memory/include/Allocators.h"

namespace CoS
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

		COS_CLASS(TerrainPatch, ZoneObject);

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

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // COSTERRAINPATCH_INCLUDED
