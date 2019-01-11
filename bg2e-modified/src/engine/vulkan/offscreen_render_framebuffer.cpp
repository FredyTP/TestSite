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

#include <bg/base/exception.hpp>

#include <bg/wnd/window_controller.hpp>

#include <memory>
#include <iostream>
#include <fstream>

#include <bg/log.hpp>

#include <bg/wnd/vk_context.hpp>

namespace bg {
namespace engine {
namespace vulkan {	

OffscreenRenderFramebuffer::OffscreenRenderFramebuffer(bg::wnd::VKContext * ctx)
    :_ctx(ctx)
    ,_colorFormat(vk::FORMAT_R8G8B8A8_UNORM)
{

}

OffscreenRenderFramebuffer::~OffscreenRenderFramebuffer() {
    _framebuffer = nullptr;
    _renderPass = nullptr;
    _depthImage = nullptr;
    _sampler = nullptr;
    _colorImage = nullptr;
}

void OffscreenRenderFramebuffer::create(uint32_t width, uint32_t height) {
    _width = width;
    _height = height;
    
    resize(width, height);
}

void OffscreenRenderFramebuffer::resize(uint32_t width, uint32_t height) {
    if (_width == width && _height == height) {
        return;
    }
    
    auto physicalDevice = _ctx->physicalDevice();
    auto device = _ctx->device();
    
    _framebuffer = nullptr;
    _renderPass = nullptr;
    _depthImage = nullptr;
    _sampler = nullptr;
    _colorImage = nullptr;

    // Color image bundle
    _colorImage = new ImageBundle();
    _colorImage->imageCreateInfo().imageType = vk::IMAGE_TYPE_2D;
    _colorImage->imageCreateInfo().flags = 0;
    _colorImage->imageCreateInfo().format = _colorFormat;
    _colorImage->imageCreateInfo().extent = { width, height, 1 };
    _colorImage->imageCreateInfo().mipLevels = 1;
    _colorImage->imageCreateInfo().arrayLayers = 1;
    _colorImage->imageCreateInfo().samples = vk::SAMPLE_COUNT_1_BIT;
    _colorImage->imageCreateInfo().tiling = vk::IMAGE_TILING_OPTIMAL;
    // We will sample directly from the color attachment
    _colorImage->imageCreateInfo().usage = vk::IMAGE_USAGE_COLOR_ATTACHMENT_BIT | vk::IMAGE_USAGE_SAMPLED_BIT;
    
    _colorImage->viewCreateInfo().viewType = vk::IMAGE_VIEW_TYPE_2D;
    _colorImage->viewCreateInfo().subresourceRange = {};
    _colorImage->viewCreateInfo().subresourceRange.aspectMask = vk::IMAGE_ASPECT_COLOR_BIT;
    _colorImage->viewCreateInfo().subresourceRange.baseMipLevel = 0;
    _colorImage->viewCreateInfo().subresourceRange.levelCount = 1;
    _colorImage->viewCreateInfo().subresourceRange.baseArrayLayer = 0;
    _colorImage->viewCreateInfo().subresourceRange.layerCount = 1;
    
    _colorImage->createAndBind(physicalDevice, device, vk::MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    // Sampler
    _sampler = new vk::Sampler();
    _sampler->createInfo.magFilter = vk::FILTER_LINEAR;
    _sampler->createInfo.minFilter = vk::FILTER_LINEAR;
    _sampler->createInfo.mipmapMode = vk::SAMPLER_MIPMAP_MODE_LINEAR;
    _sampler->createInfo.addressModeU = vk::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    _sampler->createInfo.addressModeV = vk::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    _sampler->createInfo.addressModeW = vk::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    _sampler->createInfo.mipLodBias = 0.0f;
    _sampler->createInfo.maxAnisotropy = 1.0f;
    _sampler->createInfo.minLod = 0.0f;
    _sampler->createInfo.maxLod = 1.0f;
    _sampler->createInfo.borderColor = vk::BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    _sampler->create(device);
    
    // Depth image bundle
    PhysicalDeviceHelper physicalDevHelper(physicalDevice);
    auto depthFormat = physicalDevHelper.findDepthFormat();
    
    _depthImage = new ImageBundle();
    _depthImage->imageCreateInfo().imageType = vk::IMAGE_TYPE_2D;
    _depthImage->imageCreateInfo().flags = 0;
    _depthImage->imageCreateInfo().format = depthFormat;
    _depthImage->imageCreateInfo().extent = { width, height, 1 };
    _depthImage->imageCreateInfo().mipLevels = 1;
    _depthImage->imageCreateInfo().arrayLayers = 1;
    _depthImage->imageCreateInfo().samples = vk::SAMPLE_COUNT_1_BIT;
    _depthImage->imageCreateInfo().tiling = vk::IMAGE_TILING_OPTIMAL;
    _depthImage->imageCreateInfo().usage = vk::IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    
    _depthImage->viewCreateInfo().viewType = vk::IMAGE_VIEW_TYPE_2D;
    _depthImage->viewCreateInfo().subresourceRange = {};
    _depthImage->viewCreateInfo().subresourceRange.aspectMask = vk::IMAGE_ASPECT_DEPTH_BIT | vk::IMAGE_ASPECT_STENCIL_BIT;
    _depthImage->viewCreateInfo().subresourceRange.baseMipLevel = 0;
    _depthImage->viewCreateInfo().subresourceRange.levelCount = 1;
    _depthImage->viewCreateInfo().subresourceRange.baseArrayLayer = 0;
    _depthImage->viewCreateInfo().subresourceRange.layerCount = 1;
    
    _depthImage->createAndBind(physicalDevice, device, vk::MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    // Create render pass for offscreen render
    RenderPassHelper renderPassHelper(_ctx);
    
    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = _colorFormat;
    colorAttachment.samples = vulkan::vk::SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = vulkan::vk::ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = vulkan::vk::ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = vulkan::vk::ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = vulkan::vk::ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = vulkan::vk::IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = vulkan::vk::IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    
    vk::AttachmentDescription depthAttachment;
    depthAttachment.format = depthFormat;
    depthAttachment.samples = vulkan::vk::SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = vulkan::vk::ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = vulkan::vk::ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = vulkan::vk::ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = vulkan::vk::ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = vulkan::vk::IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = vulkan::vk::IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::vector<vk::SubpassDependency> subpassDeps;
    subpassDeps.push_back(vk::SubpassDependency{
        BG_VK_SUBPASS_EXTERNAL,                         // srcSubpass
        0,                                              // dstSubpass
        vk::PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,          // srcStageMask
        vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // dstStageMask
        vk::ACCESS_MEMORY_READ_BIT,                     // srcAccessMask
        vk::ACCESS_COLOR_ATTACHMENT_READ_BIT | vk::ACCESS_COLOR_ATTACHMENT_WRITE_BIT, // dstAccessMask
        vk::DEPENDENCY_BY_REGION_BIT // dependencyFlags
    });
    
    subpassDeps.push_back(vk::SubpassDependency{
        0,                                              // srcSubpass
        BG_VK_SUBPASS_EXTERNAL,                         // dstSubpass
        vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
        vk::PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,          // dstStageMask
        vk::ACCESS_COLOR_ATTACHMENT_READ_BIT | vk::ACCESS_COLOR_ATTACHMENT_WRITE_BIT, // srcAccessMask
        vk::ACCESS_MEMORY_READ_BIT,                     // dstAccessMask
        vk::DEPENDENCY_BY_REGION_BIT                    // dependencyFlags
    });
    
    _renderPass = renderPassHelper.create(colorAttachment, depthAttachment, subpassDeps);
    
    
    // Create the framebuffer
    _framebuffer = new vk::Framebuffer();
    _framebuffer->createInfo.renderPass = _renderPass.getPtr();
    _framebuffer->createInfo.attachments.push_back(_colorImage->view());
    _framebuffer->createInfo.attachments.push_back(_depthImage->view());
    _framebuffer->createInfo.width = width;
    _framebuffer->createInfo.height = height;
    _framebuffer->createInfo.layers = 1;
    _framebuffer->create(device);
    
    _descriptorImageInfo.imageLayout = vk::IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    _descriptorImageInfo.imageView = _colorImage->view();
    _descriptorImageInfo.sampler = _sampler.getPtr();
}

}
}
}
