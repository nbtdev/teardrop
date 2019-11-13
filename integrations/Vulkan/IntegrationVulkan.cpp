/******************************************************************************
Copyright (c) 2019 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#include "IntegrationVulkan.h"

#include "RendererVulkan.h"
#include "Util/_String.h"

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

Integration::Integration()
{

}

Integration::~Integration()
{

}

void
Integration::load()
{

}

void
Integration::unload()
{

}

static Gfx::Renderer* create(int aFlags) {
    return TD_NEW Renderer(aFlags);
}

void registerIntegration()
{
    static RendererRegistration reg;
    if (!reg.mDisplayName) {
        reg.mUUID.fromString(String("8c99f4bd-325c-4a81-aaa2-e1eb295dcf31"));
        reg.mCreateFn = &create;
        reg.mDisplayName = "Vulkan";
        registerRenderer(&reg);
    }
}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
