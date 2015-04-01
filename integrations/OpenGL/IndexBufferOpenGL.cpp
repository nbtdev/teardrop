/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "IndexBufferOpenGL.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

IndexBuffer::IndexBuffer(Gfx::Submesh* aParent)
    : Gfx::IndexBuffer(aParent)
    , mBufferName(0)
{
    glGenBuffers(1, &mBufferName);
	assert(mBufferName);
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
        mCount = aIndexCount;
        mSize = aIndexSize;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount * mSize, aData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
    }

	return false;
}

void
IndexBuffer::resize(int aIndexSize, int aIndexCount)
{
    if (mBufferName) {
        mSize = aIndexSize;
        mCount = aIndexCount;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferName);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount * mSize, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void*
IndexBuffer::map(MapFlags aFlags)
{
	GLenum flags = 0;

	if (aFlags == MAP_DISCARD)
		flags |= GL_WRITE_ONLY;

	if (aFlags == MAP_READONLY)
		flags |= GL_READ_ONLY;

    if (!mIsMapped) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferName);
        void* rtn = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, flags);
        //reportGLError();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        mIsMapped = true;
        return rtn;
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

