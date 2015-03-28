/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "Gfx/IndexBuffer.h"
#include "Gfx/Allocators.h"
#include "GLHeaders.h"

namespace Teardrop {
    namespace Gfx {
        namespace OpenGL {

            class IndexBuffer : public Gfx::IndexBuffer
            {
            public:
                IndexBuffer(Gfx::Submesh* aParent);
                ~IndexBuffer();

                // Gfx::IndexBuffer implementation
                bool initialize(int indexSize, int indexCount, void* data);
                void resize(int indexSize, int indexCount);
                void* map(MapFlags flags);
                void unmap();

                GLuint bufferName();

                TD_DECLARE_ALLOCATOR();

            protected:
                GLuint mBufferName = 0;
                int mElementCount = 0;
                int mStride = 0;
                bool mIsMapped = false;
            };

        } // namespace OpenGL
    } // namespace Gfx
} // namespace Teardrop


#endif // INDEXBUFFER_H
