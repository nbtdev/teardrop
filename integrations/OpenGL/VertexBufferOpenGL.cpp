/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "VertexBufferOpenGL.h"
#include "ExtensionManager.h"

#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

VertexBuffer::VertexBuffer(Gfx::Submesh* aParent)
    : Gfx::VertexBuffer(aParent)
    , mBufferName(0)
{
    ExtensionManager::instance().genBuffers(1, &mBufferName);
}

VertexBuffer::~VertexBuffer()
{
    ExtensionManager::instance().deleteBuffers(1, &mBufferName);
}

bool
VertexBuffer::initialize(int aVertexCount, int aInitFlags, void* aData)
{
    mCount = aVertexCount;
    mInitFlags = aInitFlags;

    if (mBufferName && aData) {
        GLenum usage = GL_STATIC_DRAW;

        if (aInitFlags & INIT_DYNAMIC)
            usage = GL_DYNAMIC_DRAW;

        ExtensionManager::instance().bindBuffer(GL_ARRAY_BUFFER, mBufferName);
        ExtensionManager::instance().bufferData(GL_ARRAY_BUFFER, aVertexCount * vertexSize(), aData, usage);
        ExtensionManager::instance().bindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void
VertexBuffer::resize(int aVertexCount)
{
    if (mBufferName) {
        mCount = aVertexCount;

        GLenum usage = GL_STATIC_DRAW;

        if (mInitFlags & INIT_DYNAMIC)
            usage = GL_DYNAMIC_DRAW;

        ExtensionManager::instance().bindBuffer(GL_ARRAY_BUFFER, mBufferName);
        ExtensionManager::instance().bufferData(GL_ARRAY_BUFFER, mCount * vertexSize(), nullptr, usage);
        ExtensionManager::instance().bindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void*
VertexBuffer::map(MapFlags aFlags)
{
    if (aFlags == MAP_DISCARD) {
        if (!mIsMapped) {
            bool hasMap = ExtensionManager::instance().hasMapBuffer();
            if (hasMap) {
                ExtensionManager::instance().bindBuffer(GL_ARRAY_BUFFER, mBufferName);
                void* rtn = ExtensionManager::instance().mapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
                //reportGLError();
                ExtensionManager::instance().bindBuffer(GL_ARRAY_BUFFER, 0);
                mIsMapped = true;
                return rtn;
            }
        }
    }

    return nullptr;
}

void
VertexBuffer::unmap()
{
    if (!mIsMapped)
        return;

    bool hasMap = ExtensionManager::instance().hasMapBuffer();
    if (hasMap) {
        ExtensionManager::instance().bindBuffer(GL_ARRAY_BUFFER, mBufferName);
        ExtensionManager::instance().unmapBuffer(GL_ARRAY_BUFFER);
        ExtensionManager::instance().bindBuffer(GL_ARRAY_BUFFER, 0);
        mIsMapped = false;
    }
}

GLuint
VertexBuffer::bufferName()
{
    return mBufferName;
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop

