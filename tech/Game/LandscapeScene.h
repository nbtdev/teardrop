/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
