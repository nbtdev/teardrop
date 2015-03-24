/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXDECLARATIONOPENGL_INCLUDED)
#define VERTEXDECLARATIONOPENGL_INCLUDED

#include "Gfx/VertexDeclaration.h"
#include "Memory/Allocators.h"
#include <GL/gl.h>

namespace Teardrop
{
    namespace Gfx
    {
        class Submesh;

        namespace OpenGL {
            class VertexDeclaration : public Gfx::VertexDeclaration
            {
            public:
                VertexDeclaration(Submesh* parent);
                virtual ~VertexDeclaration();

                void rebuild();

                TD_DECLARE_ALLOCATOR();

            protected:
                GLuint mVAO = 0;
            };
        } // namespace OpenGL
    } // namespace Gfx
} // namespace Teardrop

#endif // VERTEXDECLARATIONOPENGL_INCLUDED
