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

namespace {

char const* sVkFormatToString[] = {
    "VK_FORMAT_UNDEFINED",
    "VK_FORMAT_R4G4_UNORM_PACK8",
    "VK_FORMAT_R4G4B4A4_UNORM_PACK16",
    "VK_FORMAT_B4G4R4A4_UNORM_PACK16",
    "VK_FORMAT_R5G6B5_UNORM_PACK16",
    "VK_FORMAT_B5G6R5_UNORM_PACK16",
    "VK_FORMAT_R5G5B5A1_UNORM_PACK16",
    "VK_FORMAT_B5G5R5A1_UNORM_PACK16",
    "VK_FORMAT_A1R5G5B5_UNORM_PACK16",
    "VK_FORMAT_R8_UNORM",
    "VK_FORMAT_R8_SNORM",
    "VK_FORMAT_R8_USCALED",
    "VK_FORMAT_R8_SSCALED",
    "VK_FORMAT_R8_UINT",
    "VK_FORMAT_R8_SINT",
    "VK_FORMAT_R8_SRGB",
    "VK_FORMAT_R8G8_UNORM",
    "VK_FORMAT_R8G8_SNORM",
    "VK_FORMAT_R8G8_USCALED",
    "VK_FORMAT_R8G8_SSCALED",
    "VK_FORMAT_R8G8_UINT",
    "VK_FORMAT_R8G8_SINT",
    "VK_FORMAT_R8G8_SRGB",
    "VK_FORMAT_R8G8B8_UNORM",
    "VK_FORMAT_R8G8B8_SNORM",
    "VK_FORMAT_R8G8B8_USCALED",
    "VK_FORMAT_R8G8B8_SSCALED",
    "VK_FORMAT_R8G8B8_UINT",
    "VK_FORMAT_R8G8B8_SINT",
    "VK_FORMAT_R8G8B8_SRGB",
    "VK_FORMAT_B8G8R8_UNORM",
    "VK_FORMAT_B8G8R8_SNORM",
    "VK_FORMAT_B8G8R8_USCALED",
    "VK_FORMAT_B8G8R8_SSCALED",
    "VK_FORMAT_B8G8R8_UINT",
    "VK_FORMAT_B8G8R8_SINT",
    "VK_FORMAT_B8G8R8_SRGB",
    "VK_FORMAT_R8G8B8A8_UNORM",
    "VK_FORMAT_R8G8B8A8_SNORM",
    "VK_FORMAT_R8G8B8A8_USCALED",
    "VK_FORMAT_R8G8B8A8_SSCALED",
    "VK_FORMAT_R8G8B8A8_UINT",
    "VK_FORMAT_R8G8B8A8_SINT",
    "VK_FORMAT_R8G8B8A8_SRGB",
    "VK_FORMAT_B8G8R8A8_UNORM",
    "VK_FORMAT_B8G8R8A8_SNORM",
    "VK_FORMAT_B8G8R8A8_USCALED",
    "VK_FORMAT_B8G8R8A8_SSCALED",
    "VK_FORMAT_B8G8R8A8_UINT",
    "VK_FORMAT_B8G8R8A8_SINT",
    "VK_FORMAT_B8G8R8A8_SRGB",
    "VK_FORMAT_A8B8G8R8_UNORM_PACK32",
    "VK_FORMAT_A8B8G8R8_SNORM_PACK32",
    "VK_FORMAT_A8B8G8R8_USCALED_PACK32",
    "VK_FORMAT_A8B8G8R8_SSCALED_PACK32",
    "VK_FORMAT_A8B8G8R8_UINT_PACK32",
    "VK_FORMAT_A8B8G8R8_SINT_PACK32",
    "VK_FORMAT_A8B8G8R8_SRGB_PACK32",
    "VK_FORMAT_A2R10G10B10_UNORM_PACK32",
    "VK_FORMAT_A2R10G10B10_SNORM_PACK32",
    "VK_FORMAT_A2R10G10B10_USCALED_PACK32",
    "VK_FORMAT_A2R10G10B10_SSCALED_PACK32",
    "VK_FORMAT_A2R10G10B10_UINT_PACK32",
    "VK_FORMAT_A2R10G10B10_SINT_PACK32",
    "VK_FORMAT_A2B10G10R10_UNORM_PACK32",
    "VK_FORMAT_A2B10G10R10_SNORM_PACK32",
    "VK_FORMAT_A2B10G10R10_USCALED_PACK32",
    "VK_FORMAT_A2B10G10R10_SSCALED_PACK32",
    "VK_FORMAT_A2B10G10R10_UINT_PACK32",
    "VK_FORMAT_A2B10G10R10_SINT_PACK32",
    "VK_FORMAT_R16_UNORM",
    "VK_FORMAT_R16_SNORM",
    "VK_FORMAT_R16_USCALED",
    "VK_FORMAT_R16_SSCALED",
    "VK_FORMAT_R16_UINT",
    "VK_FORMAT_R16_SINT",
    "VK_FORMAT_R16_SFLOAT",
    "VK_FORMAT_R16G16_UNORM",
    "VK_FORMAT_R16G16_SNORM",
    "VK_FORMAT_R16G16_USCALED",
    "VK_FORMAT_R16G16_SSCALED",
    "VK_FORMAT_R16G16_UINT",
    "VK_FORMAT_R16G16_SINT",
    "VK_FORMAT_R16G16_SFLOAT",
    "VK_FORMAT_R16G16B16_UNORM",
    "VK_FORMAT_R16G16B16_SNORM",
    "VK_FORMAT_R16G16B16_USCALED",
    "VK_FORMAT_R16G16B16_SSCALED",
    "VK_FORMAT_R16G16B16_UINT",
    "VK_FORMAT_R16G16B16_SINT",
    "VK_FORMAT_R16G16B16_SFLOAT",
    "VK_FORMAT_R16G16B16A16_UNORM",
    "VK_FORMAT_R16G16B16A16_SNORM",
    "VK_FORMAT_R16G16B16A16_USCALED",
    "VK_FORMAT_R16G16B16A16_SSCALED",
    "VK_FORMAT_R16G16B16A16_UINT",
    "VK_FORMAT_R16G16B16A16_SINT",
    "VK_FORMAT_R16G16B16A16_SFLOAT",
    "VK_FORMAT_R32_UINT",
    "VK_FORMAT_R32_SINT",
    "VK_FORMAT_R32_SFLOAT",
    "VK_FORMAT_R32G32_UINT",
    "VK_FORMAT_R32G32_SINT",
    "VK_FORMAT_R32G32_SFLOAT",
    "VK_FORMAT_R32G32B32_UINT",
    "VK_FORMAT_R32G32B32_SINT",
    "VK_FORMAT_R32G32B32_SFLOAT",
    "VK_FORMAT_R32G32B32A32_UINT",
    "VK_FORMAT_R32G32B32A32_SINT",
    "VK_FORMAT_R32G32B32A32_SFLOAT",
    "VK_FORMAT_R64_UINT",
    "VK_FORMAT_R64_SINT",
    "VK_FORMAT_R64_SFLOAT",
    "VK_FORMAT_R64G64_UINT",
    "VK_FORMAT_R64G64_SINT",
    "VK_FORMAT_R64G64_SFLOAT",
    "VK_FORMAT_R64G64B64_UINT",
    "VK_FORMAT_R64G64B64_SINT",
    "VK_FORMAT_R64G64B64_SFLOAT",
    "VK_FORMAT_R64G64B64A64_UINT",
    "VK_FORMAT_R64G64B64A64_SINT",
    "VK_FORMAT_R64G64B64A64_SFLOAT",
    "VK_FORMAT_B10G11R11_UFLOAT_PACK32",
    "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32",
    "VK_FORMAT_D16_UNORM",
    "VK_FORMAT_X8_D24_UNORM_PACK32",
    "VK_FORMAT_D32_SFLOAT",
    "VK_FORMAT_S8_UINT",
    "VK_FORMAT_D16_UNORM_S8_UINT",
    "VK_FORMAT_D24_UNORM_S8_UINT",
    "VK_FORMAT_D32_SFLOAT_S8_UINT",
    "VK_FORMAT_BC1_RGB_UNORM_BLOCK",
    "VK_FORMAT_BC1_RGB_SRGB_BLOCK",
    "VK_FORMAT_BC1_RGBA_UNORM_BLOCK",
    "VK_FORMAT_BC1_RGBA_SRGB_BLOCK",
    "VK_FORMAT_BC2_UNORM_BLOCK",
    "VK_FORMAT_BC2_SRGB_BLOCK",
    "VK_FORMAT_BC3_UNORM_BLOCK",
    "VK_FORMAT_BC3_SRGB_BLOCK",
    "VK_FORMAT_BC4_UNORM_BLOCK",
    "VK_FORMAT_BC4_SNORM_BLOCK",
    "VK_FORMAT_BC5_UNORM_BLOCK",
    "VK_FORMAT_BC5_SNORM_BLOCK",
    "VK_FORMAT_BC6H_UFLOAT_BLOCK",
    "VK_FORMAT_BC6H_SFLOAT_BLOCK",
    "VK_FORMAT_BC7_UNORM_BLOCK",
    "VK_FORMAT_BC7_SRGB_BLOCK",
    "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK",
    "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK",
    "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK",
    "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK",
    "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK",
    "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK",
    "VK_FORMAT_EAC_R11_UNORM_BLOCK",
    "VK_FORMAT_EAC_R11_SNORM_BLOCK",
    "VK_FORMAT_EAC_R11G11_UNORM_BLOCK",
    "VK_FORMAT_EAC_R11G11_SNORM_BLOCK",
    "VK_FORMAT_ASTC_4x4_UNORM_BLOCK",
    "VK_FORMAT_ASTC_4x4_SRGB_BLOCK",
    "VK_FORMAT_ASTC_5x4_UNORM_BLOCK",
    "VK_FORMAT_ASTC_5x4_SRGB_BLOCK",
    "VK_FORMAT_ASTC_5x5_UNORM_BLOCK",
    "VK_FORMAT_ASTC_5x5_SRGB_BLOCK",
    "VK_FORMAT_ASTC_6x5_UNORM_BLOCK",
    "VK_FORMAT_ASTC_6x5_SRGB_BLOCK",
    "VK_FORMAT_ASTC_6x6_UNORM_BLOCK",
    "VK_FORMAT_ASTC_6x6_SRGB_BLOCK",
    "VK_FORMAT_ASTC_8x5_UNORM_BLOCK",
    "VK_FORMAT_ASTC_8x5_SRGB_BLOCK",
    "VK_FORMAT_ASTC_8x6_UNORM_BLOCK",
    "VK_FORMAT_ASTC_8x6_SRGB_BLOCK",
    "VK_FORMAT_ASTC_8x8_UNORM_BLOCK",
    "VK_FORMAT_ASTC_8x8_SRGB_BLOCK",
    "VK_FORMAT_ASTC_10x5_UNORM_BLOCK",
    "VK_FORMAT_ASTC_10x5_SRGB_BLOCK",
    "VK_FORMAT_ASTC_10x6_UNORM_BLOCK",
    "VK_FORMAT_ASTC_10x6_SRGB_BLOCK",
    "VK_FORMAT_ASTC_10x8_UNORM_BLOCK",
    "VK_FORMAT_ASTC_10x8_SRGB_BLOCK",
    "VK_FORMAT_ASTC_10x10_UNORM_BLOCK",
    "VK_FORMAT_ASTC_10x10_SRGB_BLOCK",
    "VK_FORMAT_ASTC_12x10_UNORM_BLOCK",
    "VK_FORMAT_ASTC_12x10_SRGB_BLOCK",
    "VK_FORMAT_ASTC_12x12_UNORM_BLOCK",
    "VK_FORMAT_ASTC_12x12_SRGB_BLOCK",
    "VK_FORMAT_G8B8G8R8_422_UNORM",
    "VK_FORMAT_B8G8R8G8_422_UNORM",
    "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM",
    "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM",
    "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM",
    "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM",
    "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM",
    "VK_FORMAT_R10X6_UNORM_PACK16",
    "VK_FORMAT_R10X6G10X6_UNORM_2PACK16",
    "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16",
    "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16",
    "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16",
    "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16",
    "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16",
    "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16",
    "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16",
    "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16",
    "VK_FORMAT_R12X4_UNORM_PACK16",
    "VK_FORMAT_R12X4G12X4_UNORM_2PACK16",
    "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16",
    "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16",
    "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16",
    "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16",
    "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16",
    "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16",
    "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16",
    "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16",
    "VK_FORMAT_G16B16G16R16_422_UNORM",
    "VK_FORMAT_B16G16R16G16_422_UNORM",
    "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM",
    "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM",
    "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM",
    "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM",
    "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM",
    "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG",
    "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG",
    "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG",
    "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG",
    "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG",
    "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG",
    "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG",
    "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG",
    "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT",
    "VK_FORMAT_G8B8G8R8_422_UNORM_KHR",
    "VK_FORMAT_B8G8R8G8_422_UNORM_KHR",
    "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR",
    "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR",
    "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR",
    "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR",
    "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR",
    "VK_FORMAT_R10X6_UNORM_PACK16_KHR",
    "VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR",
    "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR",
    "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR",
    "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR",
    "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR",
    "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR",
    "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR",
    "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR",
    "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR",
    "VK_FORMAT_R12X4_UNORM_PACK16_KHR",
    "VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR",
    "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR",
    "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR",
    "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR",
    "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR",
    "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR",
    "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR",
    "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR",
    "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR",
    "VK_FORMAT_G16B16G16R16_422_UNORM_KHR",
    "VK_FORMAT_B16G16R16G16_422_UNORM_KHR",
    "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR",
    "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR",
    "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR",
    "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR",
    "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR",
    "VK_FORMAT_BEGIN_RANGE",
    "VK_FORMAT_END_RANGE",
    "VK_FORMAT_RANGE_SIZE",
    "VK_FORMAT_MAX_ENUM",
};

} // namespace

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

RenderWindow::RenderWindow(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, uintptr_t hWnd, SurfaceFormat fmt, int flags)
    : RenderTarget(device)
    , mInitFlags(flags)
    , mSurface(VK_NULL_HANDLE)
    , mSwapchain(VK_NULL_HANDLE)
    , mPhysicalDevice(physicalDevice)
    , mInstance(instance)
    , mImageFormat(VK_FORMAT_UNDEFINED)
    , mImages(nullptr)
    , mImageViews(nullptr)
    , mFramebuffers(nullptr)
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
    mImageCount = std::min(3U, surfaceCaps.maxImageCount ? surfaceCaps.maxImageCount : UINT32_MAX);
    mImageCount = std::max(mImageCount, surfaceCaps.minImageCount);

    uint32_t nFormats = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &nFormats, nullptr);

    VkSurfaceFormatKHR* formats = (VkSurfaceFormatKHR*)alloca(sizeof(VkSurfaceFormatKHR) * nFormats);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &nFormats, formats);

    VkFormat targetFormat = VK_FORMAT_UNDEFINED;

    switch (fmt) {
    case SURFACE_A8R8G8B8:
        targetFormat = VK_FORMAT_B8G8R8A8_UNORM;
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
    uint32_t formatIndex = 0;
    for (uint32_t i=0; i<nFormats; ++i) {
        std::cout << "        " << sVkFormatToString[formats[i].format] << std::endl;
        if (formats[i].format == targetFormat) {
            formatIndex = i;
            colorSpace = formats[i].colorSpace;
        }
    }
    std::cout << "    Using back buffer format " << sVkFormatToString[targetFormat] << std::endl;

    if (formatIndex == UINT32_MAX) {
        std::cout << "Could not find target format in supported format list" << std::endl;
        return;
    }

    VkBool32 supported;
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, mSurface, &supported);
    if (supported != VK_TRUE) {
        std::cout << "Surface not supported for presentation on this device for queue family 0" << std::endl;
        return;
    }

    mWidth = (int)surfaceCaps.currentExtent.width;
    mHeight = (int)surfaceCaps.currentExtent.height;

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
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

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

    mImageFormat = targetFormat;

    std::cout << "    Swapchain depth: " << swapchainLength() << std::endl;
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

size_t RenderWindow::currentSwapchainIndex() const
{
    return mFrameCount % imageCount();
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

VkImage RenderWindow::image() const
{
    return mImages[mCurrentImageIndex];
}

VkFormat RenderWindow::format() const
{
    return mImageFormat;
}

VkImageView RenderWindow::imageView()
{
    if (!mImageViews) {
        createImageViews();
    }

    return mImageViews[mCurrentImageIndex];
}

VkFramebuffer RenderWindow::framebuffer(VkRenderPass renderPass)
{
    if (!mFramebuffers) {
        createFrameBuffers(renderPass);
    }

    return mFramebuffers[mCurrentImageIndex];
}

uint32_t RenderWindow::frameCount() const
{
    return mFrameCount;
}

uint32_t RenderWindow::imageCount() const
{
    return mImageCount;
}

void RenderWindow::createImageViews()
{
    destroyImageViews();

    mImageViews = new VkImageView[mImageCount];

    for (uint32_t i=0; i<mImageCount; ++i) {
        mImageViews[i] = VK_NULL_HANDLE;

        VkImageViewCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.image = mImages[i];
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.layerCount = 1;
        info.format = mImageFormat;
        vkCreateImageView(mDevice, &info, getAllocationCallbacks(), &mImageViews[i]);
    }
}

void RenderWindow::destroyImageViews()
{
    if (!mImageViews) {
        return;
    }

    for (uint32_t i=0; i<mImageCount; ++i) {
        vkDestroyImageView(mDevice, mImageViews[i], getAllocationCallbacks());
    }

    delete [] mImageViews;
    mImageViews = nullptr;
}

void RenderWindow::createFrameBuffers(VkRenderPass renderPass)
{
    // unconditionally (re)create the framebuffer object; if that is not
    // desired, don't call this method
    destroyFrameBuffers();

    mFramebuffers = new VkFramebuffer[mImageCount];

    // we also need the image views; create them now if they have not already been created
    createImageViews();

    for (uint32_t i=0; i<mImageCount; ++i) {
        mFramebuffers[i] = VK_NULL_HANDLE;

        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.width = (uint32_t)width();
        info.height = (uint32_t)height();
        info.layers = 1;
        info.renderPass = renderPass;
        info.pAttachments = &mImageViews[i];
        info.attachmentCount = 1;

        vkCreateFramebuffer(mDevice, &info, getAllocationCallbacks(), &mFramebuffers[i]);
    }
}

void RenderWindow::destroyFrameBuffers()
{
    if (!mFramebuffers) {
        return;
    }

    for (uint32_t i=0; i<mImageCount; ++i) {
        vkDestroyFramebuffer(mDevice, mFramebuffers[i], getAllocationCallbacks());
    }

    delete [] mFramebuffers;
    mFramebuffers = nullptr;
}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
