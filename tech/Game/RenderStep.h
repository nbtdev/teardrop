/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERSTEP_INCLUDED)
#define RENDERSTEP_INCLUDED

#include "Memory/Allocators.h"
#include <vector>

namespace Teardrop
{
	class GfxRenderer;
	class GfxCamera;
	class ZoneObject;
	class Scene;

	class RenderStep
	{
	public:
		typedef std::vector<ZoneObject*> VisibleObjects;

		RenderStep();
		virtual ~RenderStep();

		virtual void render(
			const VisibleObjects& visibleObjects,	// objects visible through the main scene camera
			GfxRenderer* pRenderer,					// the renderer to use 
			Scene* pScene							// the scene we are rendering (some steps may use it for obtaining different sets of visible objects, for instance)
			) = 0;

		virtual void setCamera(GfxCamera* pCamera);

		TD_DECLARE_ALLOCATOR();

	protected:
		GfxCamera* m_pCamera;
	};
}

#endif // RENDERSTEP_INCLUDED
