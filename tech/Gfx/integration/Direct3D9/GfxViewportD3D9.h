/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXVIEWPORTD3D9_INCLUDED)
#define GFXVIEWPORTD3D9_INCLUDED

#include "GfxViewport.h"

namespace Teardrop
{
	class GfxViewportD3D9 : public GfxViewport
	{
	public:
		GfxViewportD3D9();
		~GfxViewportD3D9();

		bool initialize(GfxRenderTarget* pRT);
		bool destroy();
		
		DECLARE_GFX_ALLOCATOR();
	};
}

#endif // GFXVIEWPORTD3D9_INCLUDED
