/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXBUFFEROPENGL_INCLUDED)
#define VERTEXBUFFEROPENGL_INCLUDED

#include "Gfx/VertexBuffer.h"
#include "GLHeaders.h"

namespace Teardrop
{
    namespace Gfx
    {
        namespace OpenGL
        {
            class VertexBuffer : public Gfx::VertexBuffer
            {
            public:
                VertexBuffer(Gfx::Submesh* parent);
                ~VertexBuffer();

                bool initialize(int vertexCount, int initFlags, void* data=0);
                void resize(int vertexCount);
                void* map(MapFlags mapFlags);
                void unmap();

                GLuint bufferName();

                TD_DECLARE_ALLOCATOR();

            protected:
                GLuint mBufferName;
                int mInitFlags = 0;
                bool mIsMapped = false;
            };
        }
    }
}

#endif // VERTEXBUFFEROPENGL_INCLUDED
