/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "VertexDeclarationOpenGL.h"
#include "ExtensionManager.h"

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

VertexDeclaration::VertexDeclaration(Submesh* parent)
    : Gfx::VertexDeclaration(parent)
{
}

VertexDeclaration::~VertexDeclaration()
{
}

void
VertexDeclaration::rebuild()
{
    if (mVAO) {
        ExtensionManager::instance().deleteVertexArrays(1, &mVAO);
        mVAO = 0;
    }

    ExtensionManager::instance().genVertexArrays(1, &mVAO);

    // pull index and vertex information from parent Submesh, and construct
    // VAO data from that
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
