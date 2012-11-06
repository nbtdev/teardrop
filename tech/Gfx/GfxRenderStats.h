/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRENDERSTATS_INCLUDED)
#define GFXRENDERSTATS_INCLUDED

namespace Teardrop
{
	struct GfxRenderStats
	{
		GfxRenderStats();
		GfxRenderStats(const GfxRenderStats& other);
		~GfxRenderStats();

		GfxRenderStats& operator=(const GfxRenderStats& other);
		void reset();

		float frameStart;
		float frameEnd;
		size_t numTris;
		size_t numBatches;
		size_t numVerts;

		DECLARE_GFX_ALLOCATOR();
	};
}

#endif // GFXRENDERSTATS_INCLUDED