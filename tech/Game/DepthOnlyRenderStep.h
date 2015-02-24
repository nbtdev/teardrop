/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(DEPTHONLYRENDERSTEP_INCLUDED)
#define DEPTHONLYRENDERSTEP_INCLUDED

#include "RenderStep.h"

namespace Teardrop
{
	/*
		Render step that only initializes the framebuffer's depth 
		buffer, as an optimization for raster/fragment-shader-heavy 
		scenes.
	*/

	class DepthOnlyRenderStep
		: public RenderStep
	{
	public:
		DepthOnlyRenderStep();
		~DepthOnlyRenderStep();

        void render(const VisibleObjects& objects, Gfx::Renderer* pRenderer, Scene* pScene);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // DEPTHONLYRENDERSTEP_INCLUDED
