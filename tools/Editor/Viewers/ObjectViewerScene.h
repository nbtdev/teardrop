/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTVIEWERSCENE_INCLUDED)
#define OBJECTVIEWERSCENE_INCLUDED

#include "Game/Executable.h"

namespace Teardrop
{
	class Asset;

	namespace Gfx {
		class Camera;
		class Viewport;
	}

	namespace Tools
	{
		class ObjectViewerScene : public Executable
		{
		public:
			TD_CLASS(ObjectViewerScene, Executable);
			TD_POINTER_PROPERTY(FocusObject, "Object being viewed", Asset, 0);

			ObjectViewerScene();
			~ObjectViewerScene();

			void renderFrame(Gfx::Renderer* renderer);

			TD_DECLARE_ALLOCATOR();

		protected:
			Gfx::Camera* mCamera;
			Gfx::Viewport* mVP;
		};
	}
}

#endif // OBJECTVIEWERSCENE_INCLUDED
