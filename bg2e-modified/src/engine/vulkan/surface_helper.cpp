/*
*	bg2 engine license
*	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
*	of the Software, and to permit persons to whom the Software is furnished to do
*	so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
*	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
*	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/

#include <vulkan/vulkan.h>

#include <bg/engine/vulkan/vulkan_includes.hpp>

#include <bg/wnd/window_controller.hpp>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#include <memory>
#include <iostream>

#include <bg/log.hpp>

namespace bg {
namespace engine {
namespace vulkan {	

SwapchainHelper::SwapchainHelper(vk::PhysicalDevice * phDev, vk::Device * dev, vk::SurfaceKHR * surface, bg::wnd::Window * window)
    :_phDev(phDev)
    ,_dev(dev)
    ,_surface(surface)
    ,_window(window)
{
}

vk::SurfaceFormatKHR SwapchainHelper::chooseFormat() {
    using namespace bg::engine;
    vulkan::vk::SurfaceFormatKHR format;
    std::vector<vulkan::vk::SurfaceFormatKHR> formats;
    _surface->getFormats(_phDev, formats);
    if (formats.size()==1 && formats[0].format == vulkan::vk::FORMAT_UNDEFINED) {
        format.format = vulkan::vk::FORMAT_B8G8R8A8_UNORM;
        format.colorSpace = vulkan::vk::COLOR_SPACE_SRGB_NONLINEAR_KHR;
    }
    
    for (const auto & availableFormat : formats) {
        if (availableFormat.format == vulkan::vk::FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == vulkan::vk::COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            format = availableFormat;
            break;
        }
    }
    return format;
}

vk::PresentModeKHR SwapchainHelper::choosePresentMode() {
    using namespace bg::engine;
    std::vector<vulkan::vk::PresentModeKHR> presentModes;
    _surface->getPresentModes(_phDev, presentModes);
    vulkan::vk::PresentModeKHR presentMode = vulkan::vk::PRESENT_MODE_FIFO_KHR;
    for (const auto & availablePresentMode : presentModes) {
        if (availablePresentMode == vulkan::vk::PRESENT_MODE_MAILBOX_KHR) {
            presentMode = availablePresentMode;
        }
    }
    return presentMode;
}

vk::Extent2D SwapchainHelper::chooseSwapChainExtent() {
    using namespace bg::engine;
    vulkan::vk::SurfaceCapabilitiesKHR capabilities;
    _surface->getCapabilities(_phDev, capabilities);
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        vulkan::vk::Extent2D actualExtent;
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, static_cast<uint32_t>(_window->rect().width())));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, static_cast<uint32_t>(_window->rect().height())));
        return actualExtent;
    }
}

bg::engine::vulkan::vk::SwapchainKHR * SwapchainHelper::createSwapchain() {
    using namespace bg::engine;
    vulkan::QueueFamilyIndices indices = vulkan::QueueFamilyIndices::Find(_phDev, _surface);
    
    vulkan::vk::SurfaceFormatKHR format = chooseFormat();
    vulkan::vk::PresentModeKHR presentMode = choosePresentMode();
    vulkan::vk::Extent2D swapChainExtent = chooseSwapChainExtent();
    vulkan::vk::SurfaceCapabilitiesKHR surfaceCapabilities;
    _surface->getCapabilities(_phDev, surfaceCapabilities);
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
        imageCount = surfaceCapabilities.maxImageCount;
    }
    
    bg::ptr<vulkan::vk::SwapchainKHR> swapchain = new vulkan::vk::SwapchainKHR();
    std::vector<uint32_t> queueIndices;
    vulkan::vk::SharingMode sharingMode = vulkan::vk::SHARING_MODE_EXCLUSIVE;
    if (indices.graphicsFamily != indices.presentFamily) {
        queueIndices.push_back(indices.graphicsFamily);
        queueIndices.push_back(indices.presentFamily);
        sharingMode = vulkan::vk::SHARING_MODE_CONCURRENT;
    }

    swapchain->create(
        _dev,
        _surface,
        imageCount,
        format.format, format.colorSpace,
        swapChainExtent, 1,
        vulkan::vk::IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        sharingMode, queueIndices,
        surfaceCapabilities.currentTransform,
        vulkan::vk::COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        presentMode, nullptr);
    _result = swapchain.getPtr();
    return swapchain.release();
}

void SwapchainHelper::getImages(std::vector<bg::ptr<vk::Image>> &images, std::vector<bg::ptr<vk::ImageView>> & imageViews) {
    if (!_result) return;
    
    VkDevice dev = bg::native_cast<VkDevice>(_dev->vkDevice());
    VkSwapchainKHR swapchain = bg::native_cast<VkSwapchainKHR>(_result->vkSwapchain());
    std::vector<VkImage> swapchainImages;
    uint32_t count;
    vkGetSwapchainImagesKHR(dev, swapchain, &count, nullptr);
    swapchainImages.resize(count);
    vkGetSwapchainImagesKHR(dev, swapchain, &count, swapchainImages.data());
    
    for (auto img : swapchainImages) {
        images.push_back(new vk::Image(img));
    }
    
    for (auto & img : images) {
        vulkan::vk::ImageView * imgView = new vulkan::vk::ImageView();
        imgView->create(
            _dev,
            img.getPtr(),
            vulkan::vk::IMAGE_VIEW_TYPE_2D,
            _result->format(),
            vulkan::vk::ComponentMapping {
                vulkan::vk::COMPONENT_SWIZZLE_IDENTITY,
                vulkan::vk::COMPONENT_SWIZZLE_IDENTITY,
                vulkan::vk::COMPONENT_SWIZZLE_IDENTITY,
                vulkan::vk::COMPONENT_SWIZZLE_IDENTITY
            },
            vulkan::vk::ImageSubresourceRange {
                vulkan::vk::IMAGE_ASPECT_COLOR_BIT,
                0,  // baseMipLevel
                1,  // levelCount
                0,  // baseArrayLayer
                1   // layerCount
            });
        imageViews.push_back(imgView);
    }
    
}

}
}
}
