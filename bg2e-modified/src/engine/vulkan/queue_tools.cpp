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


#include <memory>
#include <iostream>

#include <bg/log.hpp>

namespace bg {
namespace engine {
namespace vulkan {	

QueueFamilyIndices QueueFamilyIndices::Find(vk::PhysicalDevice * dev, vk::SurfaceKHR * surface) {
	QueueFamilyIndices indices;
	VkPhysicalDevice vkDev = native_cast<VkPhysicalDevice>(dev->vkDevice());
	VkSurfaceKHR vkSurface = native_cast<VkSurfaceKHR>(surface->vkSurface());

	int32_t i = 0;
	for (auto & queueFamily : dev->queueFamilyProperties) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(vkDev, i, vkSurface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		++i;
	}
	
	return indices;
}

	/*
	typedef std::vector<vk::LayerProperties> LayerPropertiesVector;
	typedef std::vector<vk::ExtensionProperties> ExtensionPropertiesVector;

	inline bool success(vk::Result res) { return res == vk::Result::eSuccess; }
	inline void check(vk::Result res) { if (res != vk::Result::eSuccess) throw new bg::base::VulkanEngineException(); }
	inline void check(vk::Result res, const std::string & errorMessage) { if (res != vk::Result::eSuccess) throw new bg::base::VulkanEngineException(errorMessage); }
	inline void check(vk::Result res, const char * errorMessage) { if (res != vk::Result::eSuccess) throw new bg::base::VulkanEngineException(errorMessage); }

	BG2E_EXPORT extern void getLayerProperties(LayerPropertiesVector & layerProp);
	BG2E_EXPORT extern void getExtensionProperties(ExtensionPropertiesVector & extensionProp);
	BG2E_EXPORT extern bool checkRequiredLayers(const std::vector<std::string> & recommended, const std::vector<std::string> & required);
	BG2E_EXPORT extern bool checkRequiredExtensions(const std::vector<std::string> & recommended, const std::vector<std::string> & required);
	BG2E_EXPORT extern bool checkRequiredLayers(const std::vector<std::string> & required);
	BG2E_EXPORT extern bool checkRequiredExtensions(const std::vector<std::string> & required);


void getLayerProperties(LayerPropertiesVector & layerProp) {
	uint32_t count = 0;
	const char * errorMsg = "Error enumerating Vulkan layers";

	check(vk::enumerateInstanceLayerProperties(&count, nullptr), errorMsg);
	std::unique_ptr<vk::LayerProperties[]> layers(new vk::LayerProperties[count]);
	check(vk::enumerateInstanceLayerProperties(&count, layers.get()), errorMsg);

	for (auto i = 0; i < count; ++i) {
		layerProp.push_back(layers[i]);
	}
}

void getExtensionProperties(ExtensionPropertiesVector & extensionProp) {
	uint32_t count = 0;
	const char * errorMsg = "Error enumerating Vulkan extensions";

	check(vk::enumerateInstanceExtensionProperties(nullptr, &count, nullptr), errorMsg);
	std::unique_ptr<vk::ExtensionProperties[]> extensions(new vk::ExtensionProperties[count]);
	check(vk::enumerateInstanceExtensionProperties(nullptr, &count, extensions.get()), errorMsg);

	for (auto i = 0; i < count; ++i) {
		extensionProp.push_back(extensions[i]);
	}
}

bool checkLayers(const std::vector<std::string> & checkNames, const LayerPropertiesVector & layers) {
    for (auto & checkName : checkNames) {
        auto result = false;
        for (auto & layer : layers) {
            if (checkName==layer.layerName) {
                result = true;
                break;
            }
        }
        if (!result) {
            return false;
        }
    }
    return true;
}

bool checkExtensions(const std::vector<std::string> & checkNames, const ExtensionPropertiesVector & extensions) {
    for (auto & checkName : checkNames) {
        auto result = false;
        for (auto & extension : extensions) {
            if (checkName==extension.extensionName) {
                result = true;
                break;
            }
        }
        if (!result) {
            return false;
        }
    }
    return true;
}

bool checkRequiredLayers(const std::vector<std::string> & recommended, const std::vector<std::string> & required) {
    LayerPropertiesVector layers;
    getLayerProperties(layers);
    
    if (!checkLayers(recommended, layers)) {
        bg::log(bg::log::kWarning) << "The system does not comply with the recommended Vulkan layers." << bg::endl;
    }
    else if (!checkLayers(required, layers)) {
        bg::log(bg::log::kError) << "The system does not comply with the required Vulkan layers." << bg::endl;
        return false;
    }
    return true;
}

bool checkRequiredExtensions(const std::vector<std::string> & recommended, const std::vector<std::string> & required) {
    ExtensionPropertiesVector extensions;
    getExtensionProperties(extensions);
    
    if (!checkExtensions(recommended, extensions)) {
        bg::log(bg::log::kWarning) << "The system does not comply with the recommended Vulkan extensions." << bg::endl;
    }
    else if (!checkExtensions(required, extensions)) {
        bg::log(bg::log::kError) << "The system does not comply with the required Vulkan extensions." << bg::endl;
        return false;
    }
    return true;
}
    
bool checkRequiredLayers(const std::vector<std::string> & required) {
    LayerPropertiesVector layers;
    getLayerProperties(layers);
    
    if (!checkLayers(required, layers)) {
        bg::log(bg::log::kError) << "The system does not comply with the required Vulkan layers." << bg::endl;
        return false;
    }
    return true;
}

bool checkRequiredExtensions(const std::vector<std::string> & required) {
    ExtensionPropertiesVector extensions;
    getExtensionProperties(extensions);

    if (!checkExtensions(required, extensions)) {
        bg::log(bg::log::kError) << "The system does not comply with the required Vulkan extensions." << bg::endl;
        return false;
    }
    return true;
}
    */
}
}
}
