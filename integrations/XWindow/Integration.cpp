/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Integration.h"
#include "InputXWindow.h"

namespace Teardrop {
namespace XWindow {

Integration::Integration()
    : mInput(0)
{
    load();
}

Integration::~Integration()
{
    unload();
}

void Integration::load()
{
    // for now, just create an instance of the DirectInput singleton
    mInput = TD_NEW Input(XOpenDisplay(0));
}

void Integration::unload()
{
    // for now, just destroy and delete the instance of the DirectInput singleton
    mInput->destroy();
    delete mInput;
    mInput = 0;
}

static Input* create() {
    return TD_NEW Input(XOpenDisplay(0));
}

void registerIntegration()
{

}


} // namespace DirectInput
} // namespace Teardrop
