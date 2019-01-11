/*
 *    bg2 engine license
 *    Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *    of the Software, and to permit persons to whom the Software is furnished to do
 *    so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */


#include <bg/wnd/vk_context.hpp>

#include <bg/wnd/window_controller.hpp>

#include <bg/engine/vulkan/vulkan_includes.hpp>

#include <vulkan/vulkan.h>

namespace bg {
namespace wnd {

bool VKContext::createContext() {
	using namespace bg::engine;

	_instance = new vulkan::vk::Instance();

	std::vector<std::string> reqDeviceExtensions;
	std::vector<std::string> reqDeviceLayers;

	getRequiredLayers(_instance->enabledLayers);
	getRequiredExtensions(_instance->enabledExtensions);
	getRequiredDeviceExtensions(reqDeviceExtensions);

	if (vulkan::Debug::Enabled() && vulkan::Debug::IsSupported()) {
		vulkan::Debug::Get()->getRequiredLayers(_instance->enabledLayers);
		vulkan::Debug::Get()->getRequiredExtensions(_instance->enabledExtensions);
		reqDeviceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
	}
	
	_instance->applicationInfo.applicationName = "Test Vulkan";
	vulkan::check(_instance->create(),"Could not create application instance");

	if (vulkan::Debug::Enabled() && vulkan::Debug::IsSupported()) {
		vulkan::Debug::Get()->setMessageCallback(_instance.getPtr(), [&](bg::log::Level level, int32_t code, const std::string & msg) {
			bg::log(level) << msg << bg::endl;
		});
	}
	
	_surface = new vulkan::vk::SurfaceKHR();
	_surface->create(_instance.getPtr(),  window());

	vulkan::vk::PhysicalDevice::Vector devices;
	vulkan::vk::PhysicalDevice::Enumerate(_instance.getPtr(), devices);

	bg::log(bg::log::kDebug) << "Available devices:" << bg::endl;
	for (auto & dev : devices) {
		bg::log(bg::log::kDebug) << "  Vendor: " << dev->vendorName << bg::endl <<
			"  Device: " << dev->deviceName << bg::endl <<
			"  Memory: " << dev->vramString << bg::endl << bg::endl;
	}
	
	_physicalDevice = vulkan::chooseBestDevice(_instance.getPtr(), devices, reqDeviceExtensions, _surface.getPtr());
	if (_physicalDevice.isNull()) {
		bg::log(bg::log::kError) << "No compatible devices found" << bg::endl;
		return false;
	}
    bg::log(bg::log::kDebug) << _physicalDevice->vendorName << " " << _physicalDevice->deviceName << " choosed" << bg::endl;

	vulkan::PhysicalDeviceHelper physicalDevHelper(_physicalDevice.getPtr());
	_depthFormat = physicalDevHelper.findDepthFormat();

    _queueFamilies = vulkan::QueueFamilyIndices::Find(_physicalDevice.getPtr(), _surface.getPtr());
    
    createDevice(reqDeviceExtensions, reqDeviceLayers);
	createDeviceQueues();
	createSwapchainImages();
	createCommandPool();
	createDepthResources();
    createCommandBuffers();
	createRenderPass();
	createFramebuffers();

    return true;
}

void VKContext::makeCurrent() {
    // TODO: Make current
}

void VKContext::swapBuffers() {
    // TODO: Swap buffers
}

void VKContext::resizeSwapchain() {
    using namespace bg::engine;
    _device->waitIdle();
    
	_framebuffers.clear();
	_renderPass = nullptr;
	_depthImage = nullptr;
	_depthImageMemory = nullptr;
	_depthImageView = nullptr;
    _swapchainImageViews.clear();
    _swapchainImages.clear();
    _swapchain = nullptr;

	createSwapchainImages();
	createDepthResources();
	createRenderPass();
	createFramebuffers();
}

void VKContext::destroy() {
	_framebuffers.clear();
	_renderPass = nullptr;
	_depthImage = nullptr;
	_depthImageMemory = nullptr;
	_depthImageView = nullptr;
    _swapchainImageViews.clear();
    _swapchain = nullptr;
	_commandPool = nullptr;
	_graphicsQueue = nullptr;
	_presentQueue = nullptr;
    _device = nullptr;
	_surface = nullptr;
	bg::engine::vulkan::Debug::Destroy();
	_instance = nullptr;
}

void VKContext::getRequiredLayers(std::vector<std::string> & layers) {
}

void VKContext::getRequiredExtensions(std::vector<std::string> & extensions) {
	extensions.push_back(bg::engine::vulkan::vk::KHR_SURFACE_EXTENSION_NAME);
}

void VKContext::getRequiredDeviceExtensions(std::vector<std::string> & devExtensions) {
	devExtensions.push_back(bg::engine::vulkan::vk::KHR_SWAPCHAIN_EXTENSION_NAME);
}

void VKContext::createDevice(const std::vector<std::string> & reqDeviceExtensions, const std::vector<std::string> & reqDeviceLayers) {
    using namespace bg::engine;
    _device = new vulkan::vk::Device();
    _device->setEnabledExtensions(reqDeviceExtensions);
    _device->setEnabledLayers(reqDeviceLayers);
    _device->enabledFeatures.samplerAnisotropy = true;
    _device->create(_physicalDevice.getPtr(), std::set<int32_t>{ _queueFamilies.graphicsFamily, _queueFamilies.presentFamily });
}

void VKContext::createDeviceQueues() {
    _graphicsQueue = _device->getDeviceQueue(_queueFamilies.graphicsFamily);
    _presentQueue = _device->getDeviceQueue(_queueFamilies.presentFamily);
}

void VKContext::createSwapchainImages() {
	using namespace bg::engine;
	vulkan::SwapchainHelper swapchainHelper(_physicalDevice.getPtr(), _device.getPtr(), _surface.getPtr(), window());
	_swapchain = swapchainHelper.createSwapchain();
	if (!_swapchain.valid()) {
		throw bg::base::VulkanEngineException("Error resizing swap chain swap chain");
	}

	swapchainHelper.getImages(_swapchainImages, _swapchainImageViews);
}

void VKContext::createCommandPool() {
    using namespace bg::engine;
    _commandPool = new vulkan::vk::CommandPool();
    _commandPool->createInfo.queueFamilyIndex = _queueFamilies.graphicsFamily;
    _commandPool->createInfo.flags = vulkan::vk::COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    _commandPool->create(_device.getPtr());
}

void VKContext::createDepthResources() {
	using namespace bg::engine;
	vulkan::BufferHelper bufferHelper(_physicalDevice.getPtr(), _device.getPtr());
	vulkan::CommandBufferHelper cmdHelper(_device.getPtr(), _commandPool.getPtr(), _graphicsQueue.getPtr());
	
	bufferHelper.createImageBuffer(
		window()->rect().width(), window()->rect().height(),
		_depthFormat, vulkan::vk::IMAGE_TILING_OPTIMAL,
		vulkan::vk::IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		vulkan::vk::MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_depthImage, _depthImageMemory
	);

	_depthImageView = new vulkan::vk::ImageView();
	_depthImageView->createInfo.viewType = vulkan::vk::IMAGE_TYPE_2D;
	_depthImageView->createInfo.format = _depthFormat;
	_depthImageView->createInfo.image = _depthImage.getPtr();
	_depthImageView->createInfo.subresourceRange.aspectMask = vulkan::vk::IMAGE_ASPECT_DEPTH_BIT;
	_depthImageView->createInfo.subresourceRange.baseMipLevel = 0;
	_depthImageView->createInfo.subresourceRange.levelCount = 1;
	_depthImageView->createInfo.subresourceRange.baseArrayLayer = 0;
	_depthImageView->createInfo.subresourceRange.layerCount = 1;
	_depthImageView->create(_device.getPtr());
	cmdHelper.transitionImageLayout(
		_depthImage.getPtr(), _depthFormat,
		vulkan::vk::IMAGE_LAYOUT_UNDEFINED,
		vulkan::vk::IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	);
}

void VKContext::createCommandBuffers() {
    using namespace bg::engine;
    vulkan::vk::CommandBuffer::Allocate(
        device(),
        commandPool(),
        vulkan::vk::COMMAND_BUFFER_LEVEL_PRIMARY,
        swapchainSize(),
        _commandBuffers);
}

void VKContext::createRenderPass() {
	bg::engine::vulkan::RenderPassHelper renderPassHelper(this);
	_renderPass = renderPassHelper.create(depthFormat());
}

void VKContext::createFramebuffers() {
	bg::engine::vulkan::FramebufferHelper(this).createSwapchainFramebuffers(_renderPass.getPtr(), true, _framebuffers);
}

}
}
