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

#include <bg/engine/vulkan/vulkan_includes.hpp>

#include <bg/engine.hpp>
#include <bg/log.hpp>

#include <bg/wnd/window_controller.hpp>

#include <vulkan/vulkan.h>

namespace bg {
namespace engine {
namespace vulkan {

PhysicalDeviceHelper::PhysicalDeviceHelper(vk::PhysicalDevice * dev)
    :_dev(dev)
{

}
    
uint32_t PhysicalDeviceHelper::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
    VkPhysicalDevice vkDev = bg::native_cast<VkPhysicalDevice>(_dev->vkDevice());
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkDev, &memProperties);
    
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    
    throw bg::base::VulkanEngineException("Could not find suitable memory type for the specified device.");
}

vk::Format PhysicalDeviceHelper::findSupportedFormat(const std::vector<vk::Format> & candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
    for (auto format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(bg::native_cast<VkPhysicalDevice>(_dev->vkDevice()), static_cast<VkFormat>(format), &props);

        if (tiling == vk::IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == vk::IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw bg::base::VulkanEngineException("failed to find supported format!");
}

vk::Format PhysicalDeviceHelper::findDepthFormat() {
    return findSupportedFormat({
        vk::FORMAT_D32_SFLOAT, vk::FORMAT_D32_SFLOAT_S8_UINT, vk::FORMAT_D24_UNORM_S8_UINT
    }, vk::IMAGE_TILING_OPTIMAL, vk::FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool PhysicalDeviceHelper::hasStencilFormat(vk::Format fmt) {
	return HasStencilFormat(fmt);
}

bool PhysicalDeviceHelper::HasStencilFormat(vk::Format fmt) {
	return fmt == vk::FORMAT_D32_SFLOAT_S8_UINT || fmt == vk::FORMAT_D24_UNORM_S8_UINT;
}


}
}
}
