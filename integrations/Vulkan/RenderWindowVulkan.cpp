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
    : RenderTarget(device)
    , mInitFlags(flags)
    , mSurface(VK_NULL_HANDLE)
    , mSwapchain(VK_NULL_HANDLE)
    , mDevice(device)
    , mPhysicalDevice(physicalDevice)
    , mInstance(instance)
    , mImages(nullptr)
    , mImageCount(0)
    , mFrameCount(0)
    , mCurrentImageIndex(0)
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
        mImages[i] = images[i];
    }
}

RenderWindow::~RenderWindow()
{
    delete [] mImages;

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
    info.pImageIndices = &mCurrentImageIndex;

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

size_t RenderWindow::swapchainLength() const
{
    return imageCount();
}

void RenderWindow::waitForNext(Gfx::SynchronizationPrimitive *gpuWaitPrimitive, Gfx::SynchronizationPrimitive *cpuWaitPrimitive)
{
    assert(gpuWaitPrimitive);
    assert(cpuWaitPrimitive);
    if (!gpuWaitPrimitive || !cpuWaitPrimitive) {
        return;
    }

    Vulkan::SynchronizationPrimitive* fencePrimitive = (Vulkan::SynchronizationPrimitive*)cpuWaitPrimitive;
    Vulkan::SynchronizationPrimitive* semaphorePrimitive = (Vulkan::SynchronizationPrimitive*)gpuWaitPrimitive;
    VkFence fence = fencePrimitive->mPrimitive.fence;
    VkSemaphore semaphore = semaphorePrimitive->mPrimitive.semaphore;

    vkWaitForFences(mDevice, 1, &fence, VK_TRUE, UINT64_MAX);
    vkResetFences(mDevice, 1, &fence);

    vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &mCurrentImageIndex);
}

VkImage RenderWindow::currentImage() const
{
    return mImages[mCurrentImageIndex];
}

uint32_t RenderWindow::frameCount() const
{
    return mFrameCount;
}

uint32_t RenderWindow::imageCount() const
{
    return mImageCount;
}

void RenderWindow::createFramebuffer()
{
    // unconditionally (re)create the framebuffer object; if that is not
    // desired, don't call this method
    if (mFramebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(mDevice, mFramebuffer, getAllocationCallbacks());
        mFramebuffer = VK_NULL_HANDLE;
    }

    VkFramebufferCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.width = (uint32_t)width();
    info.height = (uint32_t)height();
    info.layers = 1;

    vkCreateFramebuffer(mDevice, &info, getAllocationCallbacks(), &mFramebuffer);
}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
