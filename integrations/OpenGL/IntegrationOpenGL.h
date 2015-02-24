/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef OPENGLINTEGRATION_H
#define OPENGLINTEGRATION_H

#include "Core/Integration.h"

namespace Teardrop
{
    namespace Gfx {
        namespace OpenGL {
            class Integration : public Teardrop::Integration
            {
            public:
                Integration();
                ~Integration();
                void load();
                void unload();
            };

            void registerIntegration();
        }
    }
}

#endif // OPENGLINTEGRATION_H
