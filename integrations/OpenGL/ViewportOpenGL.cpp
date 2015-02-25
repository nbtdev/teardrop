/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ViewportOpenGL.h"

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

Viewport::Viewport(Gfx::RenderTarget* rt)
    : Gfx::Viewport(rt)
{
}

Viewport::~Viewport()
{
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
