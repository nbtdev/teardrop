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

#include "AllocatorsVulkan.h"
#include "RenderWindowVulkan.h"

#include "Memory/Allocators.h"

#include <cstring>
#include <iostream>
#include <vector>

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

// suitable devices will have a graphics and a compute queue
uint32_t getSuitablePhysicalDeviceQueueFamilyIndex(VkPhysicalDevice physDevice)
{
    uint32_t nProps = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &nProps, nullptr);

    VkQueueFamilyProperties* queueFamilyProps = (VkQueueFamilyProperties*)alloca(sizeof(VkQueueFamilyProperties) * nProps);
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &nProps, queueFamilyProps);

    for (uint32_t i=0; i<nProps; ++i) {
        uint32_t queueFlags = queueFamilyProps[i].queueFlags;
        if (queueFlags & (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT)) {
            return i;
        }
    }

    return UINT32_MAX;
}

} // namespace

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

VkAllocationCallbacks const* getAllocationCallbacks()
{
    static VkAllocationCallbacks sAllocators = {
        nullptr,    // void*                                   pUserData;
        allocate,   // PFN_vkAllocationFunction                pfnAllocation;
        reallocate, // PFN_vkReallocationFunction              pfnReallocation;
        free,       // PFN_vkFreeFunction                      pfnFree;
        nullptr,    // PFN_vkInternalAllocationNotification    pfnInternalAllocation;
        nullptr     // PFN_vkInternalFreeNotification          pfnInternalFree;
    };

    return &sAllocators;
}

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

    char const* const targetExtensions[] = {
        "VK_KHR_display",
        "VK_KHR_get_physical_device_properties2",
        "VK_KHR_get_surface_capabilities2",
        "VK_KHR_surface",
        "VK_KHR_wayland_surface",
        "VK_KHR_xcb_surface",
        "VK_KHR_xlib_surface",
        "VK_KHR_wayland_surface",
        "VK_EXT_acquire_xlib_display",
        "VK_EXT_debug_report",
        "VK_EXT_direct_mode_display",
        "VK_EXT_display_surface_counter",
        "VK_EXT_debug_utils",
    };

    size_t const nInstanceExtensions = sizeof(targetExtensions) / sizeof(targetExtensions[0]);
    std::vector<char const*> extensions;

    std::cout << "Initializing Vulkan Renderer:" << std::endl;
    std::cout << "    Available Extensions:" << std::endl;
    for (uint32_t i=0; i<nProps; ++i) {
        std::cout << "        " << props[i].extensionName << std::endl;

        for (size_t j=0; j<nInstanceExtensions; ++j) {
            if (!strcmp(targetExtensions[j], props[i].extensionName)) {
                extensions.push_back(targetExtensions[j]);
                continue;
            }
        }
    }

    VkInstanceCreateInfo instanceCreateInfo = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        nullptr,
        0,
        nullptr,
        0,
        nullptr,
        (uint32_t)extensions.size(),
        extensions.data()
    };

    if (VK_SUCCESS != vkCreateInstance(&instanceCreateInfo, getAllocationCallbacks(), &mInstance)) {
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

    std::cout << "    Available Physical Devices:" << nPhysDevices << std::endl;

    uint32_t firstActualGPU = UINT32_MAX;
    uint32_t suitableQueueFamilyIndex = UINT32_MAX;

    for (uint32_t i=0; i<nPhysDevices; ++i) {
        VkPhysicalDeviceProperties props = {};
        vkGetPhysicalDeviceProperties(physDevices[i], &props);
        std::cout << "        " << props.deviceName << std::endl;

        if(props.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
         props.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            continue;
        }

        suitableQueueFamilyIndex = getSuitablePhysicalDeviceQueueFamilyIndex(physDevices[i]);
        if (firstActualGPU == UINT32_MAX && suitableQueueFamilyIndex != UINT32_MAX) {
            firstActualGPU = i;
        }
    }

    if (firstActualGPU == UINT32_MAX) {
        std::cout << "No suitable physical devices found" << std::endl;
        return;
    }

    // pick the first one in the list for now
    // TODO: let the user choose/set their device choice
    mPhysicalDevice = physDevices[firstActualGPU];

    char const* const targetDeviceExtensions[] = {
        "VK_KHR_swapchain",
    };

    size_t const nDeviceExtensions = sizeof(targetDeviceExtensions) / sizeof(targetDeviceExtensions[0]);
    extensions.clear();

    // find out what device extensions we have available
    uint32_t nDeviceProps = 0;
    vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &nDeviceProps, nullptr);

    VkExtensionProperties* deviceProps = (VkExtensionProperties*)alloca(sizeof(VkExtensionProperties) * nDeviceProps);
    vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &nDeviceProps, deviceProps);

    std::cout << "    Available Device Extensions: " << nDeviceProps << std::endl;
    for (uint32_t i=0; i<nDeviceProps; ++i) {
        std::cout << "        " << deviceProps[i].extensionName << std::endl;

        for (size_t j=0; j<nDeviceExtensions; ++j) {
            if (!strcmp(targetDeviceExtensions[j], deviceProps[i].extensionName)) {
                extensions.push_back(targetDeviceExtensions[j]);
                continue;
            }
        }
    }

    VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.queueCount = 1;
    deviceQueueCreateInfo.queueFamilyIndex = suitableQueueFamilyIndex;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
    deviceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();

    if (VK_SUCCESS != vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, getAllocationCallbacks(), &mDevice)) {
        std::cout << "Failed to create device" << std::endl;
        return;
    }
}

Renderer::~Renderer()
{
    if (mDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(mDevice, getAllocationCallbacks());
    }

    if (mInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(mInstance, getAllocationCallbacks());
    }
}

std::shared_ptr<Gfx::RenderTarget> Renderer::createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags)
{
    std::shared_ptr<Gfx::RenderTarget> rt;

    rt.reset(new Vulkan::RenderWindow(mInstance, mPhysicalDevice, mDevice, hWnd, fmt, flags));

    return rt;
}

std::shared_ptr<Gfx::RenderTarget> Renderer::createRenderTexture(int w, int h, SurfaceFormat fmt, int flags)
{

}

std::weak_ptr<CommandBuffer> Renderer::createCommandBuffer(bool reusable)
{

}

std::weak_ptr<RenderPass> Renderer::createRenderPass()
{

}

std::weak_ptr<Pipeline> Renderer::createPipeline(PipelineType type)
{

}

SynchronizationPrimitive* Renderer::createSynchronizationPrimitive(SynchronizationPrimitiveType type, bool signaled)
{

}

std::weak_ptr<CommandQueue> Renderer::getCommandQueue(size_t index)
{

}

size_t Renderer::getCommandQueueCount() const
{

}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
