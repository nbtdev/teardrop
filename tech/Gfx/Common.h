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

#if !defined(GFX_COMMON_INCLUDED)
#define GFX_COMMON_INCLUDED

#include "Util/UUID.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Renderer;


		enum InitFlags {
			INIT_FRAMEBUFFER_ALPHA = (1<<0),
			INIT_ENABLE_VSYNC = (1<<1),
			INIT_ENABLE_STENCIL_BUFFER = (1<<2),
			INIT_ENABLE_DEPTH_BUFFER = (1<<3),
			INIT_FULLSCREEN = (1<<4),
			INIT_ENABLE_NVPERFHUD = (1<<5),
		};
	
		enum SurfaceFormat {
			// 4 channels, 1 byte per channel
			SURFACE_A8R8G8B8,
			// 3 channels, 1 byte per channel
			SURFACE_R8G8B8,
			// 4 channels, 1 single-precision float per channel
			SURFACE_RGBA_32F,
		};

		// Renderer implementation registration
		typedef Renderer*(*RendererCreateFn)(int /*flags*/);

		struct RendererRegistration
		{
			RendererRegistration();
			Renderer* create(int aFlags) const;
			
			// (required) unique implementation ID
			UUID mUUID;
			// (required) creation function that returns an instance of Renderer
            RendererCreateFn mCreateFn = nullptr;
			// (required) short name suitable for list entries
            const char* mDisplayName = nullptr;
			// (optional) description of renderer implementation
            const char* mDescription = nullptr;
			// (internal use only)
            RendererRegistration* mNext = nullptr;
		};

		void registerRenderer(RendererRegistration* registration);
		const RendererRegistration* rendererRegistrations();
	}
}

#endif // GFX_COMMON_INCLUDED
