/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(LANDSCAPEZONE_INCLUDED)
#define LANDSCAPEZONE_INCLUDED

#include "Asset/LandscapeAsset.h"
#include "Memory/Allocators.h"
#include "Math/Vector2.h"
#include "Package/Executable.h"

namespace Teardrop
{
	class LandscapeScene : public Executable
	{
	public:
		TD_CLASS(LandscapeScene, Executable);
		TD_CLASS_CREATABLE();

		TD_POINTER_PROPERTY(LandscapeAsset, "Landscape asset for this scene", LandscapeAsset, 0);
		//TD_POINTER_PROPERTY(HUD, "Heads-up display for this scene", HUD, 0);
		TD_PROPERTY(CastShadows, "Landscape casts shadows (default: yes)", bool, true, 0);
		TD_PROPERTY(ReceiveShadows, "Landscape receives shadows (default: yes)", bool, true, 0);
		TD_COMPLEX_PROPERTY(Tiling, "Number of tiles in X and Z ", Vector2, "(0,0)", 0);

		LandscapeScene();
		~LandscapeScene();

		bool initialize();
		bool destroy();
		void tick();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // LANDSCAPEZONE_INCLUDED
