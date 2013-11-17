/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(STATICMESHVIEWER_INCLUDED)
#define STATICMESHVIEWER_INCLUDED

#include "Package/Executable.h"
#include "Util/Timer.h"

namespace Teardrop
{
	class StaticMeshAsset;
	class OrbitCamController;

	namespace Tools
	{
		class StaticMeshViewer : public Executable
		{
		public:
			TD_CLASS(StaticMeshViewer, Executable);
			TD_POINTER_PROPERTY(StaticMeshAsset, "Static mesh being viewed", StaticMeshAsset, 0);

			StaticMeshViewer();
			~StaticMeshViewer();

			bool initialize();
			void tick();
			void renderFrame(Gfx::Renderer* renderer);

			TD_DECLARE_ALLOCATOR();

		protected:
			Timer mTimer;
			OrbitCamController* mController;
		};
	}
}

#endif // STATICMESHVIEWER_INCLUDED
