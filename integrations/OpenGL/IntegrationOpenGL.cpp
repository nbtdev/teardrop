/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "IntegrationOpenGL.h"
#include "RendererOpenGL.h"
#include "Util/_String.h"

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

Integration::Integration()
{

}

Integration::~Integration()
{

}

void
Integration::load()
{

}

void
Integration::unload()
{

}

static Gfx::Renderer* create() {
    return TD_NEW Renderer();
}

void registerIntegration()
{
    static RendererRegistration reg;
    if (!reg.mDisplayName) {
        reg.mUUID.fromString(String("a6cef76c-814a-4e49-83db-50280c43aaa8"));
        reg.mCreateFn = &create;
        reg.mDisplayName = "OpenGL 4";
        registerRenderer(&reg);
    }
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
