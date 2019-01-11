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

#ifndef _bg2e_vulkan_vulkan_includes_hpp_
#define _bg2e_vulkan_vulkan_includes_hpp_

#include <bg/export.hpp>

#include <bg/base/exception.hpp>
#include <bg/base/context.hpp>
#include <bg/base/image.hpp>
#include <bg/engine/vulkan/wrapper.hpp>

#include <bg/system/path.hpp>

#include <bg/log.hpp>

#include <vector>
#include <string>
#include <functional>

namespace bg {

namespace wnd {
class VKContext;
}

namespace engine {
namespace vulkan {

inline bool success(vk::Result r) { return r == vk::SUCCESS; }
inline void check(vk::Result r, const std::string & message) { if (!success(r)) throw new bg::base::VulkanEngineException(message); }
inline void check(vk::Result r, const char * message) { if (!success(r)) throw new bg::base::VulkanEngineException(message); }
inline void check(vk::Result r) { if (!success(r)) throw new bg::base::VulkanEngineException(); }

BG2E_EXPORT extern bg::wnd::VKContext * context(bg::base::Context *);

struct BG2E_EXPORT QueueFamilyIndices {
	int32_t graphicsFamily = -1;
	int32_t presentFamily = -1;

	inline bool isComplete() const { return graphicsFamily >= 0 && presentFamily >= 0; }

	static QueueFamilyIndices Find(vk::PhysicalDevice *, vk::SurfaceKHR *);
};

vk::PhysicalDevice * chooseBestDevice(vk::Instance *, vk::PhysicalDevice::Vector & available, const std::vector<std::string> & reqExtensions, vk::SurfaceKHR * surface);

class BG2E_EXPORT Debug {
public:
	typedef std::function<void(bg::log::Level level, int32_t, const std::string &)> MessageCallback;

	static bool Enabled();
	static bool IsSupported();
	static Debug * Get();
	static void Destroy();

	void getRequiredLayers(std::vector<std::string> & layers);
	void getRequiredExtensions(std::vector<std::string> & extensions);

	bool setMessageCallback(vk::Instance *, MessageCallback);

	void logMessage(bg::log::Level level, int32_t code, const std::string & msg);

protected:
	Debug();
	virtual ~Debug();

	static Debug * s_debug;

	MessageCallback _messageCallback;
	vk::Instance * _instance = nullptr;
};

class BG2E_EXPORT SwapchainHelper {
public:
    SwapchainHelper(vk::PhysicalDevice * phDev, vk::Device * dev, vk::SurfaceKHR * surface, bg::wnd::Window * window);
    vk::SurfaceFormatKHR chooseFormat();
    vk::PresentModeKHR choosePresentMode();
    vk::Extent2D chooseSwapChainExtent();
    vk::SwapchainKHR * createSwapchain();
    
    void getImages(std::vector<bg::ptr<vk::Image>> &images, std::vector<bg::ptr<vk::ImageView>> & imageViews);
    
    inline vk::SwapchainKHR * result() { return _result; }
    
    
protected:
    vk::PhysicalDevice * _phDev;
    vk::Device * _dev;
    vk::SurfaceKHR * _surface;
    bg::wnd::Window * _window;
    
    vk::SwapchainKHR * _result = nullptr;
};

class BG2E_EXPORT ShaderHelper {
public:
    ShaderHelper(vk::Device *);
    
    void loadFromFile(const std::string & path, vk::ShaderModule::Buffer & result);
    void loadFromFile(const bg::system::Path & path, vk::ShaderModule::Buffer & result);
    vk::ShaderModule * loadFromFile(const std::string & path);
    vk::ShaderModule * loadFromFile(const bg::system::Path & path);
    
protected:
    vk::Device * _dev;
};

class BG2E_EXPORT RenderPassHelper {
public:
	RenderPassHelper(bg::wnd::VKContext *);

	vk::RenderPass * create(vk::Format depthFormat);
	vk::RenderPass * create();

    // Advanced render pass creation
    vk::RenderPass * create(
        const vk::AttachmentDescription & colorAttachment,
        const vk::AttachmentDescription & depthAttachment,
        const std::vector<vk::SubpassDependency> & subpassDep
        );
    
protected:
	bg::wnd::VKContext * _ctx;
};

class BG2E_EXPORT FramebufferHelper {
public:
    FramebufferHelper(bg::wnd::VKContext *);
    
    void createSwapchainFramebuffers(vk::RenderPass * rp, bool depthBufferEnabled, std::vector<bg::ptr<vk::Framebuffer>> & result);
    void createSwapchainFramebuffers(vk::RenderPass * rp, bool depthBufferEnabled, std::vector<vk::Framebuffer*> & result);
    
protected:
    bg::wnd::VKContext * _ctx;
};

class BG2E_EXPORT PhysicalDeviceHelper {
public:
    PhysicalDeviceHelper(vk::PhysicalDevice * dev);
    
    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    vk::Format findSupportedFormat(const std::vector<vk::Format> & candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
    vk::Format findDepthFormat();
    bool hasStencilFormat(vk::Format fmt);
	static bool HasStencilFormat(vk::Format fmt);
    
protected:
    vk::PhysicalDevice * _dev;
};

class BG2E_EXPORT BufferHelper {
public:
    BufferHelper(vk::PhysicalDevice * phDev, vk::Device * dev);
    
    void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::SharingMode sharingMode, vk::MemoryPropertyFlags properties, bg::ptr<vk::Buffer> & buffer, bg::ptr<vk::DeviceMemory> & memory);
    void mapMemory(vk::DeviceMemory * mem, vk::DeviceSize offset, vk::MemoryMapFlags flags, const void * data, size_t size);
    void createStagingBuffer(vk::CommandPool * cmdPool, vk::Queue * queue, vk::BufferUsageFlags usage, const void * data, vk::DeviceSize size, bg::ptr<vk::Buffer> & buffer, bg::ptr<vk::DeviceMemory> & memory);
    void copyBuffer(vk::CommandPool * cmdPool, vk::Queue * queue, vk::Buffer * src, vk::Buffer * dst, vk::DeviceSize size);
    
    void createImageBuffer(vk::CommandPool * cmdPool, vk::Queue * queue, bg::base::Image *, bg::ptr<vk::Image> & textureImage, bg::ptr<vk::DeviceMemory> & textureImageMemory);
    void createImageBuffer(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, bg::ptr<vk::Image> & image, bg::ptr<vk::DeviceMemory> & imageMemory);
    
protected:
    vk::PhysicalDevice * _phDev;
    vk::Device * _dev;
};

class BG2E_EXPORT CommandBufferHelper {
public:
    CommandBufferHelper(vk::Device *, vk::CommandPool * pool, vk::Queue * queue);
    
    void executeCommands(std::function<void(vk::CommandBuffer *)>);
    
    void beginCmdBuffer();
    void transitionImageLayout(vk::Image * img, vk::Format fmt, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
    void copyBufferToImage(vk::Buffer * buffer, vk::Image * image, uint32_t width, uint32_t height, uint32_t layers = 1);
    void endCmdBuffer();
    
protected:
    vk::Device * _device;
    vk::CommandPool * _pool;
    vk::Queue * _queue;
    
    bg::ptr<vk::CommandBuffer> _cmdBuffer;
};

class BG2E_EXPORT TextureHelper : public BufferHelper {
public:
    TextureHelper(vk::PhysicalDevice * phDev, vk::Device * dev);
    
    vk::ImageView::ImageViewCreateInfo imageViewCreateInfo;
    vk::Sampler::SamplerCreateInfo samplerCreateInfo;
    
    inline void createTexture(vk::CommandPool * cmdPool, vk::Queue * queue, const bg::system::Path & imgPath, bg::ptr<vk::Image> & texImage, bg::ptr<vk::DeviceMemory> & texMemory, bg::ptr<vk::ImageView> & imgView, bg::ptr<vk::Sampler> & sampler) {
        createTexture(cmdPool, queue, imgPath.text(), texImage, texMemory, imgView, sampler);
    }
    void createTexture(vk::CommandPool * cmdPool, vk::Queue * queue, const std::string & imgPath, bg::ptr<vk::Image> & texImage, bg::ptr<vk::DeviceMemory> & texMemory, bg::ptr<vk::ImageView> & imgView, bg::ptr<vk::Sampler> & sampler);
    void createTexture(vk::CommandPool * cmdPool, vk::Queue * queue, bg::base::Image * img, bg::ptr<vk::Image> & texImage, bg::ptr<vk::DeviceMemory> & texMemory, bg::ptr<vk::ImageView> & imgView, bg::ptr<vk::Sampler> & sampler);
    void createCubemap(
        vk::CommandPool * cmdPool,
        vk::Queue * queue,
        const std::string & posX,
        const std::string & negX,
        const std::string & posY,
        const std::string & negY,
        const std::string & posZ,
        const std::string & negZ,
        bg::ptr<vk::Image> & texImage,
        bg::ptr<vk::DeviceMemory> & texMemory,
        bg::ptr<vk::ImageView> & imgView,
        bg::ptr<vk::Sampler> & sampler);
    void createCubemap(
        vk::CommandPool * cmdPool,
        vk::Queue * queue,
        bg::base::Image * posX,
        bg::base::Image * negX,
        bg::base::Image * posY,
        bg::base::Image * negY,
        bg::base::Image * posZ,
        bg::base::Image * negZ,
        bg::ptr<vk::Image> & texImage,
        bg::ptr<vk::DeviceMemory> & texMemory,
        bg::ptr<vk::ImageView> & imgView,
        bg::ptr<vk::Sampler> & sampler);
};


class BG2E_EXPORT PipelineLayoutManager : public bg::base::ReferencedPointer {
public:
	PipelineLayoutManager(bg::wnd::VKContext * ctx);

	// Descriptor set layouts
	// call newLayout() to create a new descriptorSetLayout, and addBinding to add bindings to the current layout
	void newLayout();
	void addBinding(uint32_t binding, vk::DescriptorType type, uint32_t descriptorCount, vk::ShaderStageFlags shaderStages);

	// Configure descriptor pool
	void addDescriptorPoolSize(vk::DescriptorType type, uint32_t count);
	void setDescriptorPoolMaxSets(uint32_t maxSets);

	// Create a descriptor set for the current layout configuration
	vk::DescriptorSet * createDescriptorSet();


	// Update descriptor set functions
	void addWriteDescriptorSet(uint32_t binding, uint32_t dstArrayElem, vk::DescriptorType type, uint32_t descCount, vk::DescriptorBufferInfo * bufferInfos);
	void addWriteDescriptorSet(uint32_t binding, uint32_t dstArrayElem, vk::DescriptorType type, uint32_t descCount, vk::DescriptorImageInfo * imgInfos);

	// The following functions does the same as the previous ones, but without using DescriptorBufferInfo and DescriptorImageInfo structures
	void addWriteDescriptorSet(uint32_t binding, uint32_t dstArrayElem, vk::DescriptorType type, uint32_t descCount, vk::Buffer * buffer, uint32_t offset, uint32_t range);
	void addWriteDescriptorSet(uint32_t binding, uint32_t dstArrayElem, vk::DescriptorType type, uint32_t descCount, vk::Sampler * sampler, vk::ImageView * imgView, vk::ImageLayout imgLayout);
	void updateDescriptorSet(vk::DescriptorSet *);

	// Create the structures configured with the above functions
	void create();

	inline vk::PipelineLayout * pipelineLayout() { return _pipelineLayout.getPtr(); }
	inline vk::DescriptorPool * descriptorPool() { return _descriptorPool.getPtr(); }
	inline const std::vector<bg::ptr<vk::DescriptorSetLayout>> & descriptorSetLayouts() const { return _descriptorSetLayouts; }
	inline std::vector<bg::ptr<vk::DescriptorSetLayout>> & descriptorSetLayouts() { return _descriptorSetLayouts; }
	//inline vk::DescriptorSet * descriptorSet() { return _descriptorSet.getPtr(); }
	inline vk::DescriptorSetLayout * currentLayout() { return _descriptorSetLayouts.size()>0 ? _descriptorSetLayouts.back().getPtr() : nullptr; }

protected:
	virtual ~PipelineLayoutManager();

	bg::ptr<vk::PipelineLayout> _pipelineLayout;
	bg::ptr<vk::DescriptorPool> _descriptorPool;
	std::vector<bg::ptr<vk::DescriptorSetLayout>> _descriptorSetLayouts;
	//bg::ptr<vk::DescriptorSet> _descriptorSet;

	// Internal use
	bg::wnd::VKContext * _ctx;
	std::vector<vk::WriteDescriptorSet> _writeDescriptorSets;
	std::vector<vk::DescriptorBufferInfo*> _descriptorBufferInfos;
	std::vector<vk::DescriptorImageInfo*> _descriptorImageInfos;

	inline void createPoolIfNeeded() {
		if (_descriptorPool.isNull()) _descriptorPool = new vk::DescriptorPool();
	}

	
};

struct SyncObjects {
    std::vector<bg::ptr<vk::Semaphore>> imageAvailableSemaphore;
    std::vector<bg::ptr<vk::Semaphore>> renderFinishedSemaphore;
    std::vector<bg::ptr<vk::Fence>> frameFences;
    
    void destroy() {
        imageAvailableSemaphore.clear();
        renderFinishedSemaphore.clear();
        frameFences.clear();
    }
};

class BG2E_EXPORT RenderManager : public bg::base::ReferencedPointer {
public:
    RenderManager(bg::wnd::VKContext *);
    
    void create();
    void waitForPreviousFrame();
    void presentNextFrame();
    
    inline uint32_t currentFrame() const { return _currentFrame; }
    inline uint32_t inFlightFrames() const { return _inFlightFrames; }

protected:
    virtual ~RenderManager();
    
    bg::wnd::VKContext * _ctx;
    
    SyncObjects _syncObjects;
    uint32_t _currentFrame;
    uint32_t _inFlightFrames;
};

class ImageBundle : public bg::base::ReferencedPointer {
public:
    ImageBundle() {
        _view = new vk::ImageView();
        _memory = new vk::DeviceMemory();
        _image = new vk::Image();
    }
    ImageBundle(vk::Image * i, vk::DeviceMemory * m, vk::ImageView * v) :_image(i), _memory(m), _view(v) {}
    
    inline vk::Image::ImageCreateInfo & imageCreateInfo() { return _image->createInfo; }
    inline vk::DeviceMemory::MemoryAllocateInfo & memoryAllocateInfo() { return _memory->allocateInfo; }
    /*
     *  NOTE: The viewCreateInfo().format and viewCreateInfo().image parameters are set
     *  automatically from the imageCreateInfo settings
     */
    inline vk::ImageView::ImageViewCreateInfo & viewCreateInfo() { return _view->createInfo; }
    
    inline void createAndBind(vk::PhysicalDevice * phDev, vk::Device * dev, vk::MemoryPropertyFlags memProp, vk::DeviceSize bindOffset = 0) {
        PhysicalDeviceHelper phDevHelper(phDev);
        _image->create(dev);
        
        auto memReq = _image->getImageMemoryRequirements();
        _memory->allocateInfo.allocationSize = memReq.size;
        _memory->allocateInfo.memoryTypeIndex = phDevHelper.findMemoryType(memReq.memoryTypeBits, memProp);
        _memory->allocateMemory(dev);
        _memory->bindImageMemory(_image.getPtr(), bindOffset);
       
        _view->createInfo.format = _image->createInfo.format;
        _view->createInfo.image = _image.getPtr();
        _view->create(dev);
    }
    
    inline vk::Image * image() { return _image.getPtr(); }
    inline vk::DeviceMemory * memory() { return _memory.getPtr(); }
    inline vk::ImageView * view() { return _view.getPtr(); }
    inline const vk::Image * image() const { return _image.getPtr(); }
    inline const vk::DeviceMemory * memory() const { return _memory.getPtr(); }
    inline const vk::ImageView * view() const { return _view.getPtr(); }
    
protected:
    virtual ~ImageBundle() {
        _view = nullptr;
        _memory = nullptr;
        _image = nullptr;
    }
    
    bg::ptr<vk::Image> _image;
    bg::ptr<vk::DeviceMemory> _memory;
    bg::ptr<vk::ImageView> _view;
};

class BG2E_EXPORT OffscreenRenderFramebuffer : public bg::base::ReferencedPointer {
public:
    OffscreenRenderFramebuffer(bg::wnd::VKContext *);
    
    inline void setColorFormat(vk::Format fmt) { _colorFormat = fmt; }
    inline vk::Format colorFormat() const { return _colorFormat; }
    
    void create(uint32_t width, uint32_t height);
    void resize(uint32_t width, uint32_t height);
    
    inline ImageBundle * colorImage() { return _colorImage.getPtr(); }
    inline vk::Sampler * sampler() { return _sampler.getPtr(); }
    inline ImageBundle * depthImage() { return _depthImage.getPtr(); }
    inline vk::RenderPass * renderPass() { return _renderPass.getPtr(); }
    inline vk::Framebuffer * framebuffer() { return _framebuffer.getPtr(); }
    
    inline const ImageBundle * colorImage() const { return _colorImage.getPtr(); }
    inline const vk::Sampler * sampler() const { return _sampler.getPtr(); }
    inline const ImageBundle * depthImage() const { return _depthImage.getPtr(); }
    inline const vk::RenderPass * renderPass() const { return _renderPass.getPtr(); }
    inline const vk::Framebuffer * framebuffer() const { return _framebuffer.getPtr(); }
    
    inline const vk::DescriptorImageInfo & descriptorImageInfo() const { return _descriptorImageInfo; }
    
protected:
    virtual ~OffscreenRenderFramebuffer();
    
    bg::wnd::VKContext * _ctx;
    
    vk::Format _colorFormat;
    
    int _width = 0;
    int _height = 0;
    bg::ptr<ImageBundle> _colorImage;
    bg::ptr<vk::Sampler> _sampler;
    bg::ptr<ImageBundle> _depthImage;
    bg::ptr<vk::RenderPass> _renderPass;
    bg::ptr<vk::Framebuffer> _framebuffer;
    vk::DescriptorImageInfo _descriptorImageInfo;
};

class BG2E_EXPORT OffscreenRenderer : public bg::base::ReferencedPointer {
public:
    struct ShaderModuleData {
        bg::system::Path shaderPath;
        std::string entryPoint;
        vk::ShaderStageFlagBits type;
    };
    
    OffscreenRenderer(bg::wnd::VKContext *);
    
    inline void addShader(const ShaderModuleData & data) { _shaderModules.push_back(data); }
    inline void setPrimitiveTopology(vk::PrimitiveTopology t) { _primitiveTopology = t; }
    inline vk::Pipeline::VertexInputStateCreateInfo & vertexInputState() { return _vertexInputState; }
    inline PipelineLayoutManager * pipelineLayoutManager() { return _layoutMgr.getPtr(); }
    
    void create(int w, int h);
    void resize(int w, int h);
    
    inline OffscreenRenderFramebuffer * framebufferResources() { return _framebufferResources.getPtr(); };
    
    inline vk::CommandBuffer * commandBuffer() { return _commandBuffer.getPtr(); }
    inline vk::Semaphore * semaphore() { return _cmdBuffSemapore.getPtr(); }
    inline vk::Pipeline * pipeline() { return _pipeline.getPtr(); }
    
protected:
    virtual ~OffscreenRenderer();
    
    bg::wnd::VKContext * _ctx;
    
    bg::ptr<OffscreenRenderFramebuffer> _framebufferResources;
    bg::ptr<vk::CommandBuffer> _commandBuffer;
    bg::ptr<vk::Semaphore> _cmdBuffSemapore;
    bg::ptr<PipelineLayoutManager> _layoutMgr;
    bg::ptr<vk::Pipeline> _pipeline;
    
    std::vector<ShaderModuleData> _shaderModules;
    vk::PrimitiveTopology _primitiveTopology;
    vk::Pipeline::VertexInputStateCreateInfo _vertexInputState;
    
    void createPipeline(uint32_t w, uint32_t h);
    
};

}
}
}

#endif	// _bg2e_export_hpp_


