/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef XWINDOW_INTEGRATION_H
#define XWINDOW_INTEGRATION_H

#include "Core/Integration.h"

namespace Teardrop
{
    namespace XWindow {

        class Input;

        class Integration : public Teardrop::Integration
        {
        public:
            Integration();
            ~Integration();
            void load();
            void unload();

        private:
            Input* mInput;
        };

        void registerIntegration();
    }
}

#endif // XWINDOW_INTEGRATION_H
