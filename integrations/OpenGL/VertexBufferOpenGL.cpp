/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "VertexBufferOpenGL.h"

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

VertexBuffer::VertexBuffer(Gfx::Submesh* aParent)
    : Gfx::VertexBuffer(aParent)
    , mBufferName(0)
{
    glGenBuffers(1, &mBufferName);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &mBufferName);
}

bool
VertexBuffer::initialize(int aVertexCount, int aInitFlags, void* aData)
{
    mCount = aVertexCount;
    mInitFlags = aInitFlags;

    if (mBufferName) {
        GLenum usage = GL_STATIC_DRAW;

        if (aInitFlags & INIT_DYNAMIC)
            usage = GL_DYNAMIC_DRAW;

        glBindBuffer(GL_ARRAY_BUFFER, mBufferName);
        glBufferData(GL_ARRAY_BUFFER, aVertexCount * vertexSize(), aData, usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
    }

	return false;
}

void
VertexBuffer::resize(int aVertexCount)
{
    if (mBufferName) {
        mCount = aVertexCount;

        GLenum usage = GL_STATIC_DRAW;

        if (mInitFlags & INIT_DYNAMIC)
            usage = GL_DYNAMIC_DRAW;

        glBindBuffer(GL_ARRAY_BUFFER, mBufferName);
        glBufferData(GL_ARRAY_BUFFER, mCount * vertexSize(), nullptr, usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void*
VertexBuffer::map(MapFlags aFlags)
{
    if (aFlags == MAP_DISCARD) {
        if (!mIsMapped) {
            glBindBuffer(GL_ARRAY_BUFFER, mBufferName);
            void* rtn = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            //reportGLError();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            mIsMapped = true;
            return rtn;
        }
    }

    return nullptr;
}

void
VertexBuffer::unmap()
{
    if (!mIsMapped)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, mBufferName);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    mIsMapped = false;
}

GLuint
VertexBuffer::bufferName()
{
    return mBufferName;
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop

