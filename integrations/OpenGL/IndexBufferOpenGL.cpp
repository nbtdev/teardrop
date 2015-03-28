/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "IndexBufferOpenGL.h"

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

IndexBuffer::IndexBuffer(Gfx::Submesh* aParent)
    : Gfx::IndexBuffer(aParent)
    , mBufferName(0)
{
    glGenBuffers(1, &mBufferName);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &mBufferName);
}

bool
IndexBuffer::initialize(int aIndexSize, int aIndexCount, void* aData)
{
    if (mBufferName) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferName);
        mElementCount = aIndexCount;
        mStride = aIndexSize;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndexCount * aIndexSize, aData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
    }

	return false;
}

void
IndexBuffer::resize(int aIndexSize, int aIndexCount)
{
    if (mBufferName) {
        mStride = aIndexSize;
        mElementCount = aIndexCount;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferName);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mElementCount * mStride, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void*
IndexBuffer::map(MapFlags aFlags)
{
    if (aFlags == MAP_DISCARD) {
        if (!mIsMapped) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferName);
            void* rtn = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
            //reportGLError();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            mIsMapped = true;
            return rtn;
        }
    }

    return nullptr;
}

void
IndexBuffer::unmap()
{
    if (!mIsMapped)
        return;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferName);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    mIsMapped = false;
}

GLuint
IndexBuffer::bufferName()
{
    return mBufferName;
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop

