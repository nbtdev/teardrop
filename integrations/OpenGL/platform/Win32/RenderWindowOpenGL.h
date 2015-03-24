/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERWINDOWOPENGL_INCLUDED)
#define RENDERWINDOWOPENGL_INCLUDED

#include "RenderTargetOpenGL.h"
#include "GLHeaders.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace OpenGL
		{
			class RenderWindow : public RenderTarget
			{
			public:
				RenderWindow(HWND aParent);
				RenderWindow(HWND aParent, int aFlags);
				~RenderWindow();

				// remaining overrides that OpenGL::RenderTarget does not implement
				void present();
				void resize(int w, int h);
				void setCurrent();

				// RenderWindow-specific methods
				void initContext(HWND aDummy); // internal use only
				int initFlags();

				TD_DECLARE_ALLOCATOR();

			protected:
				int mInitFlags = 0;
				HWND mParent = 0;
				HWND mWindow = 0;
				HGLRC mContext = 0;
				HDC mDC = 0;
			};

		} // namespace OpenGL
	} // namespace Gfx
} // namespace Teardrop

#endif // RENDERWINDOWOPENGL_INCLUDED
