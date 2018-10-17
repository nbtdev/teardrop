/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(STATICMESHVIEWER_INCLUDED)
#define STATICMESHVIEWER_INCLUDED

#include "Core/Executable.h"
#include "Util/Timer.h"

namespace Teardrop
{
	class StaticMeshAsset;
	class OrbitCamController;

	namespace Gfx
	{
		class Viewport;
	}

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
			void renderFrame(Gfx::Renderer* renderer, Gfx::RenderTarget* rt);

			TD_DECLARE_ALLOCATOR();

		protected:
			Timer mTimer;
			OrbitCamController* mController;
			Gfx::Viewport* mVP;
		};
	}
}

#endif // STATICMESHVIEWER_INCLUDED
