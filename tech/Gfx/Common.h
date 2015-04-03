/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
