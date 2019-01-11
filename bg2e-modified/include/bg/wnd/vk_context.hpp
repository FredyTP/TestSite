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


#ifndef _bg2e_wnd_vk_context_hpp_
#define _bg2e_wnd_vk_context_hpp_

#include <bg/wnd/context.hpp>

#include <bg/engine/vulkan/vulkan_includes.hpp>

#include <vector>
#include <string>

namespace bg {
namespace wnd {

class Window;

class BG2E_EXPORT VKContext : public Context {
public:
	VKContext(Window * window) :Context(window) {
	}
    
    bool createContext();
    void makeCurrent();
    void swapBuffers();
    
    void destroy();
    
    void resizeSwapchain();

	// Instance and devices
	inline bg::engine::vulkan::vk::Instance * instance() { return _instance.getPtr(); }
    inline bg::engine::vulkan::vk::PhysicalDevice * physicalDevice() { return _physicalDevice.getPtr(); }
    inline bg::engine::vulkan::vk::Device * device() { return _device.getPtr(); }
    inline bg::engine::vulkan::vk::SurfaceKHR * surface() { return _surface.getPtr(); }

	// Swapchain resources
    inline bg::engine::vulkan::vk::SwapchainKHR * swapchain() { return _swapchain.getPtr(); }
    inline std::vector<bg::ptr<bg::engine::vulkan::vk::Image>> & swapchainImages() { return _swapchainImages; }
    inline std::vector<bg::ptr<bg::engine::vulkan::vk::ImageView>> & swapchainImageViews() { return _swapchainImageViews; }
    inline uint32_t swapchainSize() const { return static_cast<uint32_t>(_swapchainImages.size()); }
	inline bg::engine::vulkan::vk::RenderPass * renderPass() { return _renderPass.getPtr(); }
	inline std::vector<bg::ptr<bg::engine::vulkan::vk::Framebuffer>> & framebuffers() { return _framebuffers; }
	inline bg::engine::vulkan::vk::Framebuffer * framebuffer(uint32_t frame) { return _framebuffers[frame].getPtr(); }
	

	// Depth resources
	inline bg::engine::vulkan::vk::Image * depthImage() { return _depthImage.getPtr(); }
	inline bg::engine::vulkan::vk::DeviceMemory * depthImageMemory() { return _depthImageMemory.getPtr(); }
	inline bg::engine::vulkan::vk::ImageView * depthImageView() { return _depthImageView.getPtr(); }
	inline bg::engine::vulkan::vk::Format depthFormat() { return _depthFormat; }

	// Queues
	inline const bg::engine::vulkan::QueueFamilyIndices & queueFamilies() const { return _queueFamilies; }
	inline bg::engine::vulkan::QueueFamilyIndices & queueFamilies() { return _queueFamilies; }
	inline bg::engine::vulkan::vk::Queue * graphicsQueue() { return _graphicsQueue.getPtr(); }
	inline bg::engine::vulkan::vk::Queue * presentQueue() { return _presentQueue.getPtr(); }
    
	// Command pool and command buffers
	inline bg::engine::vulkan::vk::CommandPool * commandPool() { return _commandPool.getPtr(); }
    inline bg::engine::vulkan::vk::CommandBuffer * commandBuffer(uint32_t i) { return _commandBuffers[i].getPtr(); }

protected:
	virtual ~VKContext() {}
    
	bool _validationLayersEnabled = false;

	bg::ptr<bg::engine::vulkan::vk::Instance> _instance;
	bg::ptr<bg::engine::vulkan::vk::PhysicalDevice> _physicalDevice;
	bg::ptr<bg::engine::vulkan::vk::SurfaceKHR> _surface;
    bg::ptr<bg::engine::vulkan::vk::Device> _device;
    bg::ptr<bg::engine::vulkan::vk::SwapchainKHR> _swapchain;
    std::vector<bg::ptr<bg::engine::vulkan::vk::Image>> _swapchainImages;
    std::vector<bg::ptr<bg::engine::vulkan::vk::ImageView>> _swapchainImageViews;
	bg::ptr<bg::engine::vulkan::vk::RenderPass> _renderPass;
	bg::ptr<bg::engine::vulkan::vk::Image> _depthImage;
	bg::ptr<bg::engine::vulkan::vk::DeviceMemory> _depthImageMemory;
	bg::ptr<bg::engine::vulkan::vk::ImageView> _depthImageView;
	bg::engine::vulkan::vk::Format _depthFormat;
	std::vector<bg::ptr<bg::engine::vulkan::vk::Framebuffer>> _framebuffers;

	// Queues
	bg::engine::vulkan::QueueFamilyIndices _queueFamilies;
	bg::ptr<bg::engine::vulkan::vk::Queue> _graphicsQueue;
	bg::ptr<bg::engine::vulkan::vk::Queue> _presentQueue;

	// Command pool
	bg::ptr<bg::engine::vulkan::vk::CommandPool> _commandPool;
 
    std::vector<bg::ptr<bg::engine::vulkan::vk::CommandBuffer>> _commandBuffers;

	virtual void getRequiredLayers(std::vector<std::string> &);
	virtual void getRequiredExtensions(std::vector<std::string> &);
	virtual void getRequiredDeviceExtensions(std::vector<std::string> &);

    void createDevice(const std::vector<std::string> & reqDeviceExtensions, const std::vector<std::string> & reqDeviceLayers);
    void createDeviceQueues();
	void createSwapchainImages();
    void createCommandPool();
	void createDepthResources();
    void createCommandBuffers();
	void createRenderPass();
	void createFramebuffers();
};


}
}

#endif
