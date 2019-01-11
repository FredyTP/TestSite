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

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData)
{
	bg::log::Level level;
	switch (flags) {
	case VK_DEBUG_REPORT_ERROR_BIT_EXT:
		level = bg::log::kError;
		break;
	case VK_DEBUG_REPORT_WARNING_BIT_EXT:
		level = bg::log::kWarning;
		break;
	case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
		level = bg::log::kDebug;
		break;
	default:
		level = bg::log::kDebug;
	}
	Debug::Get()->logMessage(level, code, msg);
	return VK_FALSE;
}

VkDebugReportCallbackEXT g_callback;
bool g_callbackInitialized = false;
bool g_supportInitialized = false;
bool g_supported = false;

Debug * Debug::s_debug = nullptr;

bool Debug::Enabled() {
#ifdef BG2E_DEBUG
	return true;
#else
	return false;
#endif
}

bool Debug::IsSupported() {
	if (!g_supportInitialized) {
		g_supportInitialized = true;
		g_supported = vk::LayerProperties::CheckSupport("VK_LAYER_LUNARG_standard_validation") &&
			vk::ExtensionProperties::CheckSupport(vulkan::vk::EXT_DEBUG_REPORT_EXTENSION_NAME);
	}
	if (!g_supported) {
		bg::log(bg::log::kWarning) << "This system does not support vulkan debug." << bg::endl;
		return false;
	}
	return true;
}

Debug * Debug::Get() {
	if (!s_debug) {
		s_debug = new Debug();
	}
	return s_debug;
}

void Debug::Destroy() {
	if (s_debug) {
		delete s_debug;
		s_debug = nullptr;
	}
}

Debug::Debug() {

}

Debug::~Debug() {
	if (_instance && g_callbackInitialized) {
		auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)_instance->getProcAddr("vkDestroyDebugReportCallbackEXT");
		if (vkDestroyDebugReportCallbackEXT) {
			vkDestroyDebugReportCallbackEXT(native_cast<VkInstance>(_instance->vkInstance()), g_callback, nullptr);
		}
		g_callbackInitialized = false;
		g_supportInitialized = false;
		g_supported = false;
		g_callback = nullptr;
	}
}

void Debug::getRequiredLayers(std::vector<std::string> & layers) {
	layers.push_back("VK_LAYER_LUNARG_standard_validation");
}

void Debug::getRequiredExtensions(std::vector<std::string> & extensions) {
	extensions.push_back(vk::EXT_DEBUG_REPORT_EXTENSION_NAME);
}

bool Debug::setMessageCallback(vk::Instance * instance, MessageCallback cb) {
	_messageCallback = cb;
	_instance = instance;
	if (!g_callbackInitialized) {
		g_callbackInitialized = true;
		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		createInfo.pfnCallback = debugCallback;
		createInfo.pUserData = this;

		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)instance->getProcAddr("vkCreateDebugReportCallbackEXT");
		if (vkCreateDebugReportCallbackEXT) {
			vkCreateDebugReportCallbackEXT(native_cast<VkInstance>(instance->vkInstance()), &createInfo, nullptr, &g_callback);
		}
		else {
			bg::log(bg::log::kWarning) << "Debug callback extension not present" << bg::endl;
			return false;
		}
	}
	return true;
}

void Debug::logMessage(bg::log::Level level, int32_t code, const std::string & msg) {
	if (_messageCallback) {
		_messageCallback(level, code, msg);
	}
}

}
}
}