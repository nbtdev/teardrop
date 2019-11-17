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

#if defined(WIN32)
    #define VK_USE_PLATFORM_WIN32_KHR 1
#else // WIN32
    #include <alloca.h>
    #include <X11/Xlib-xcb.h>

    #define VK_USE_PLATFORM_XCB_KHR 1
    #define VK_USE_PLATFORM_XLIB_KHR 1
    #define VK_USE_PLATFORM_XLIB_XRANDR_EXT 1
    #define VK_USE_PLATFORM_WAYLAND_KHR 1
#endif // WIN3

#include "RenderWindowVulkan.h"

#include "AllocatorsVulkan.h"
#include "CommandQueueVulkan.h"
#include "RenderTargetVulkan.h"
#include "SynchronizationPrimitiveVulkan.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

RenderWindow::RenderWindow(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, uintptr_t hWnd, SurfaceFormat fmt, int flags)
    : mInitFlags(flags)
    , mSurface(VK_NULL_HANDLE)
    , mSwapchain(VK_NULL_HANDLE)
    , mDevice(device)
    , mPhysicalDevice(physicalDevice)
    , mInstance(instance)
    , mImageCount(0)
    , mImages(nullptr)
    , mSemaphores(nullptr)
    , mFences(nullptr)
    , mFrameCount(0)
{
#if defined(__linux__)
    Display *dpy = XOpenDisplay(NULL);
    {
        VkXcbSurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.connection = XGetXCBConnection(dpy);
        createInfo.window = (xcb_window_t)hWnd;

        VkResult r = vkCreateXcbSurfaceKHR(instance, &createInfo, getAllocationCallbacks(), &mSurface);
        if (VK_SUCCESS != r) {
            std::cout << "Could not create Xcb surface" << std::endl;
            return;
        }
    }
#else // __linux__
#endif // __linux__

    VkSurfaceCapabilitiesKHR surfaceCaps = {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, mSurface, &surfaceCaps);

    // we are trying for 3 images in the swapchain
    mImageCount = std::min(3U, surfaceCaps.maxImageCount);
    mImageCount = std::max(mImageCount, surfaceCaps.minImageCount);

    uint32_t nFormats = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &nFormats, nullptr);

    VkSurfaceFormatKHR* formats = (VkSurfaceFormatKHR*)alloca(sizeof(VkSurfaceFormatKHR) * nFormats);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &nFormats, formats);

    uint32_t formatIndex = UINT32_MAX;

    VkFormat targetFormat = VK_FORMAT_UNDEFINED;

    switch (fmt) {
    case SURFACE_A8R8G8B8:

        targetFormat = VK_FORMAT_B8G8R8A8_SRGB;
        break;
    case SURFACE_R8G8B8:
        targetFormat = VK_FORMAT_R8G8B8_UINT;
        break;
    case SURFACE_RGBA_32F:
        targetFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
        break;
    }

    std::cout << "    Supported formats: " << nFormats << std::endl;
    VkColorSpaceKHR colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    for (uint32_t i=0; i<nFormats; ++i) {
        if (formats[i].format == targetFormat) {
            formatIndex = i;
            colorSpace = formats[i].colorSpace;
        }
    }

    if (formatIndex == UINT32_MAX) {
        std::cout << "Could not find target format in supported format list" << std::endl;
        return;
    }

    // create a swapchain with mImages images, each in the desired format
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mSurface;
    createInfo.imageExtent.width = surfaceCaps.currentExtent.width;
    createInfo.imageExtent.height = surfaceCaps.currentExtent.height;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.minImageCount = mImageCount;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageFormat = targetFormat;
    createInfo.imageColorSpace = colorSpace;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.imageArrayLayers = 1;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

    VkResult r = vkCreateSwapchainKHR(device, &createInfo, getAllocationCallbacks(), &mSwapchain);
    if (VK_SUCCESS != r) {
        std::cout << "Could not create swapchain" << std::endl;
        return;
    }

    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, nullptr);

    VkImage* images = (VkImage*)alloca(sizeof(VkImage) * imageCount);
    vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, images);

    mImageCount = imageCount;
    mImages = new VkImage[mImageCount];
    for (uint32_t i=0; i<mImageCount; ++i) {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(device, &fenceCreateInfo, getAllocationCallbacks(), &mFences[i]);

        VkSemaphoreCreateInfo semCreateInfo = {};
        semCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(device, &semCreateInfo, getAllocationCallbacks(), &mSemaphores[i]);

        mImages[i] = images[i];
    }
}

RenderWindow::~RenderWindow()
{
    for (uint32_t i=0; i<mImageCount; ++i) {
        vkDestroyFence(mDevice, mFences[i], getAllocationCallbacks());
        vkDestroySemaphore(mDevice, mSemaphores[i], getAllocationCallbacks());
    }

    if (mSwapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(mDevice, mSwapchain, getAllocationCallbacks());
    }

    if (mSurface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(mInstance, mSurface, getAllocationCallbacks());
    }
}

void RenderWindow::resize(int w, int h)
{

}

void RenderWindow::setCurrent()
{

}

void RenderWindow::unsetCurrent()
{

}

VkImage RenderWindow::acquireNextImage()
{
    uint32_t currentIndex = mFrameCount % mImageCount;
    uint32_t imageIndex = UINT32_MAX;

    vkWaitForFences(mDevice, 1, &mFences[currentIndex], VK_TRUE, UINT64_MAX);
    vkResetFences(mDevice, 1, &mFences[currentIndex]);

    VkResult r = vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX, mSemaphores[currentIndex], VK_NULL_HANDLE, &imageIndex);

    if (VK_SUCCESS != r) {
        return VK_NULL_HANDLE;
    }

    return mImages[imageIndex];
}

void RenderWindow::presentQueue(Gfx::CommandQueue* queue,
                                Gfx::SynchronizationPrimitive* gpuWaitPrimitives, size_t gpuWaitCount,
                                Gfx::SynchronizationPrimitive* cpuWaitPrimitive
                                )
{
    assert(queue);
    if (!queue) {
        return;
    }

    Vulkan::CommandQueue* vulkanQueue = (Vulkan::CommandQueue*)queue;

    if (cpuWaitPrimitive && cpuWaitPrimitive->mType != SYNCH_TYPE_GPU_TO_CPU) {
        return;
    }

    for (size_t i=0; i<gpuWaitCount && gpuWaitPrimitives; ++i) {
        if (gpuWaitPrimitives[i].mType != SYNCH_TYPE_GPU_TO_GPU) {
            return;
        }
    }

    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pSwapchains = &mSwapchain;
    info.swapchainCount = 1;
    info.waitSemaphoreCount = (uint32_t)gpuWaitCount;

    if (gpuWaitCount && gpuWaitPrimitives) {
        VkSemaphore* semaphores = (VkSemaphore*)alloca(sizeof(VkSemaphore) * gpuWaitCount);
        for (size_t i=0; i<gpuWaitCount; ++i) {
            Vulkan::SynchronizationPrimitive& primitive = (Vulkan::SynchronizationPrimitive&)gpuWaitPrimitives[i];
            semaphores[i] = primitive.mPrimitive.semaphore;
        }

        info.pWaitSemaphores = semaphores;
    }

    vkQueuePresentKHR(vulkanQueue->queue(), &info);

    mFrameCount++;
}


} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
