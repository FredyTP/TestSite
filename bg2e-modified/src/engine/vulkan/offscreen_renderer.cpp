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


OffscreenRenderer::OffscreenRenderer(bg::wnd::VKContext * ctx)
    :_ctx(ctx)
{
    _layoutMgr = new PipelineLayoutManager(_ctx);
}

OffscreenRenderer::~OffscreenRenderer() {
    _pipeline = nullptr;
    _framebufferResources = nullptr;
    _cmdBuffSemapore = nullptr;
    _commandBuffer = nullptr;
}

void OffscreenRenderer::create(int w, int h) {
    vk::CommandBuffer::Allocate(
        _ctx->device(),
        _ctx->commandPool(),
        vulkan::vk::COMMAND_BUFFER_LEVEL_PRIMARY,
        _commandBuffer
    );
    
    _cmdBuffSemapore = new vk::Semaphore(_ctx->device());
    
    _framebufferResources = new OffscreenRenderFramebuffer(_ctx);
    _framebufferResources->create(w, h);
    
    createPipeline(w, h);
}

void OffscreenRenderer::resize(int w, int h) {
    _framebufferResources->resize(w, h);
    createPipeline(w, h);
}

void OffscreenRenderer::createPipeline(uint32_t w, uint32_t h) {
    _pipeline = new vk::Pipeline();
    
    
    // Shaders
    ShaderHelper shaderHelper(_ctx->device());
    for (auto & shader : _shaderModules) {
        bg::ptr<vk::ShaderModule> shaderModule = shaderHelper.loadFromFile(shader.shaderPath);
        _pipeline->shaderStages.push_back({
            shaderModule.getPtr(),
            shader.entryPoint,
            shader.type
        });
    }
    
    _pipeline->vertexInputState = _vertexInputState;
    
    _pipeline->inputAssemblyState.topology = _primitiveTopology;
    _pipeline->inputAssemblyState.primitiveRestartEnable = false;
    
    vk::Viewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = static_cast<float>(w);
    viewport.height = static_cast<float>(h);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    vk::Rect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent.width = w;
    scissor.extent.height = h;
    
    _pipeline->setPipelineLayout(_layoutMgr->pipelineLayout());
    
    _pipeline->viewportState.viewports.push_back(viewport);
    _pipeline->viewportState.scissors.push_back(scissor);
    
    _pipeline->rasterizationState.cullMode = vk::CULL_MODE_NONE;
    _pipeline->rasterizationState.frontFace = vk::FRONT_FACE_COUNTER_CLOCKWISE;
    
    _pipeline->depthStencilState.depthTestEnable = true;
    _pipeline->depthStencilState.depthWriteEnable = true;
    _pipeline->depthStencilState.depthCompareOp = vk::COMPARE_OP_LESS;
    _pipeline->depthStencilState.depthBoundsTestEnable = false;
    _pipeline->depthStencilState.minDepthBounds = 0.0f;
    _pipeline->depthStencilState.maxDepthBounds = 1.0f;
    _pipeline->depthStencilState.stencilTestEnable = false;
    
    
    // TODO: Blend state
    //vk::PipelineColorBlendAttachmentState blendState = {};
    //blendState.blendEnable = false;
    //blendState.srcColorBlendFactor = vulkanBlendFactor(_blendOptions.srcColorBlendFactor);
    //blendState.dstColorBlendFactor = vulkanBlendFactor(_blendOptions.dstColorBlendFactor);
    //blendState.colorBlendOp = vulkanBlendOperation(_blendOptions.colorBlendOp);
    //blendState.srcAlphaBlendFactor = vulkanBlendFactor(_blendOptions.srcAlphaBlendFactor);
    //blendState.dstAlphaBlendFactor = vulkanBlendFactor(_blendOptions.dstAlphaBlendFactor);
    //blendState.alphaBlendOp = vulkanBlendOperation(_blendOptions.alphaBlendOp);
    //blendState.colorWriteMask = vk::COLOR_COMPONENT_R_BIT |
    //                            vk::COLOR_COMPONENT_G_BIT |
    //                            vk::COLOR_COMPONENT_B_BIT |
    //                            vk::COLOR_COMPONENT_A_BIT;
    //_pipeline->colorBlendState.attachments.push_back(blendState);

    _pipeline->setRenderPass(_framebufferResources->renderPass());
    _pipeline->create(_ctx->device());
}

}
}
}
