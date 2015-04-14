/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RendererOpenGL.h"
#include "RenderWindowOpenGL.h"

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

std::shared_ptr<Gfx::RenderTarget>
Renderer::createRenderWindow(uintptr_t hWnd, SurfaceFormat /*fmt*/, int flags)
{
	std::shared_ptr<OpenGL::RenderWindow> renderWindow(TD_NEW OpenGL::RenderWindow((HWND)hWnd, flags));
	mRenderTargets.push_back(renderWindow);

	return renderWindow;
}

std::shared_ptr<Gfx::RenderTarget>
Renderer::createDummyContext()
{
	return std::shared_ptr<Gfx::RenderTarget>(TD_NEW RenderWindow(NULL, 0, true));
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
