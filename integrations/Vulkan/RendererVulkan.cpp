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

#include "RendererVulkan.h"

#include "Memory/Allocators.h"

#if defined(WIN32)
#else // WIN32
#include <alloca.h>
#endif // WIN32

#include <iostream>

namespace {

using namespace Teardrop;

void* allocate(
    void*                                       pUserData,
    size_t                                      size,
    size_t                                      alignment,
    VkSystemAllocationScope                     allocationScope)
{
    return TD_ALLOCATE(DEFAULT, (size_t)size);
}

void* reallocate(
    void*                                       pUserData,
    void*                                       pOriginal,
    size_t                                      size,
    size_t                                      alignment,
    VkSystemAllocationScope                     allocationScope)
{
    return TD_REALLOCATE(DEFAULT, pOriginal, (size_t)size);
}

void free(
    void*                                       pUserData,
    void*                                       pMemory)
{
    TD_DEALLOCATE(DEFAULT, pMemory);
}

void allocNotify(
    void*                                       pUserData,
    size_t                                      size,
    VkInternalAllocationType                    allocationType,
    VkSystemAllocationScope                     allocationScope)
{

}

void freeNotify(
    void*                                       pUserData,
    size_t                                      size,
    VkInternalAllocationType                    allocationType,
    VkSystemAllocationScope                     allocationScope)
{

}

VkAllocationCallbacks sAllocators = {
    nullptr,    // void*                                   pUserData;
    allocate,   // PFN_vkAllocationFunction                pfnAllocation;
    reallocate, // PFN_vkReallocationFunction              pfnReallocation;
    free,       // PFN_vkFreeFunction                      pfnFree;
    nullptr,    // PFN_vkInternalAllocationNotification    pfnInternalAllocation;
    nullptr     // PFN_vkInternalFreeNotification          pfnInternalFree;
};

}

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

Renderer::Renderer(int /*flags*/)
    : mInstance(VK_NULL_HANDLE)
    , mDevice(VK_NULL_HANDLE)
{
    uint32_t nProps = 0;
    if (VK_SUCCESS != vkEnumerateInstanceExtensionProperties(nullptr, &nProps, nullptr)) {
        return;
    }

    VkExtensionProperties* props = (VkExtensionProperties*)alloca(sizeof(VkExtensionProperties) * nProps);
    if (VK_SUCCESS != vkEnumerateInstanceExtensionProperties(nullptr, &nProps, props)) {
        std::cout << "Failed to enumerate instance extension properties" << std::endl;
        return;
    }

    std::cout << "Initializing Vulkan Renderer:" << std::endl;
    std::cout << "    Available Extensions:" << std::endl;
    for (uint32_t i=0; i<nProps; ++i) {
        std::cout << "        " << props[i].extensionName << std::endl;
    }

    VkInstanceCreateInfo instanceCreateInfo = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        nullptr,
        0,
        nullptr,
        0,
        nullptr,
        0,
        nullptr
    };

    if (VK_SUCCESS != vkCreateInstance(&instanceCreateInfo, &sAllocators, &mInstance)) {
        std::cout << "Failed to create instance" << std::endl;
        return;
    }

    uint32_t nPhysDevices = 0;
    if (VK_SUCCESS != vkEnumeratePhysicalDevices(mInstance, &nPhysDevices, nullptr)) {
        std::cout << "Failed to enumerate physical devices" << std::endl;
        return;
    }

    VkPhysicalDevice* physDevices = (VkPhysicalDevice*)alloca(sizeof(VkPhysicalDevice) * nPhysDevices);
    if (VK_SUCCESS != vkEnumeratePhysicalDevices(mInstance, &nPhysDevices, physDevices)) {
        std::cout << "Failed to enumerate physical devices" << std::endl;
        return;
    }

    std::cout << "    Available Physical Devices:" << std::endl;
    for (uint32_t i=0; i<nPhysDevices; ++i) {
        VkPhysicalDeviceProperties props = {};
        vkGetPhysicalDeviceProperties(physDevices[i], &props);
        std::cout << "        " << props.deviceName << std::endl;
    }
}

Renderer::~Renderer()
{
    if (mInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(mInstance, &sAllocators);
    }
}

std::shared_ptr<Gfx::RenderTarget> Renderer::createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags)
{

}

std::shared_ptr<Gfx::RenderTarget> Renderer::createRenderTexture(int w, int h, SurfaceFormat fmt, int flags)
{

}

void Renderer::beginFrame()
{

}

void Renderer::beginScene(Camera* camera, Viewport* vp)
{

}

void Renderer::beginObject(const Matrix44& worldXf)
{

}

void Renderer::apply(Pipeline* pipeline)
{

}

void Renderer::render(Submesh* submesh)
{

}

void Renderer::endObject()
{

}

void Renderer::endScene()
{

}

void Renderer::endFrame()
{

}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
