/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SCENERENDERER_INCLUDED)
#define SCENERENDERER_INCLUDED

#include "Memory/Allocators.h"
#include <vector>

namespace Teardrop
{
    namespace Gfx {
        class Renderer;
        class Camera;
    }

    class ZoneObject;
	class RenderStep;
	class Scene;

	/*
		Class to manage multiple steps in a single-frame render. 
		Note that the render call takes the "main scene cam" as 
		a parameter; this is an optimization to allow this class
		to cache the list of objects visible with this camera, in
		anticipation of reuse of same by multiple different 
		RenderStep implementations. At the least, it will need to
		be done once per frame anyway. All RenderStep::render() 
		implementations take this list as a parameter, but they are
		not required to use it, of course.
	*/

	class SceneRenderer
	{
		typedef std::vector<RenderStep*> RenderSteps;
		RenderSteps m_renderSteps;
        Gfx::Camera* m_pCamera;

	public:
		SceneRenderer();
		virtual ~SceneRenderer();

		// when you add a step, this class will take ownership of it and clean it up on destruction;
		// RenderSteps are executed in the order in which they are added.
		virtual void addStep(RenderStep* pStep);

		typedef std::vector<ZoneObject*> ZoneObjects;
		virtual void render(
			const ZoneObjects& visbleObjects,	// list of objects visible through the camera
            Gfx::Renderer* pRenderer,				// renderer to use to render the scene
			Scene* pScene,						// passthru to RenderSteps
            Gfx::Camera* pViewCam	= 0				// main player view camera
			);

        void setCamera(Gfx::Camera* pCam) { m_pCamera = pCam; }

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SCENERENDERER_INCLUDED
