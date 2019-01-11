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



#include <bg/render/vulkan_shader_inputs.hpp>
#include <bg/engine/vulkan.hpp>

#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/vk_context.hpp>

namespace bg {
namespace render {

inline uint32_t vulkanStages(uint32_t inputStages) {
    using namespace bg::engine;
    uint32_t stages = 0;
    if (inputStages & ShaderInputs::kInputVertexShader) {
        stages |= vulkan::vk::SHADER_STAGE_VERTEX_BIT;
    }
    if (inputStages & ShaderInputs::kInputFragmentShader) {
        stages |= vulkan::vk::SHADER_STAGE_FRAGMENT_BIT;
    }
    return stages;
}

VulkanShaderInputs::VulkanShaderInputs(bg::base::Context * ctx)
    :ShaderInputs(ctx)
{

}

VulkanShaderInputs::~VulkanShaderInputs() {

}

void VulkanShaderInputs::createLayout() {
    using namespace bg::engine;
    auto ctx = vulkan::context(context());
    _pipelineLayoutManager = new vulkan::PipelineLayoutManager(ctx);
    
    for (auto & ub : _uniformBuffers) {
        _pipelineLayoutManager->addDescriptorPoolSize(vulkan::vk::DESCRIPTOR_TYPE_UNIFORM_BUFFER, ctx->swapchainSize());
        _pipelineLayoutManager->addBinding(ub.binding, vulkan::vk::DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, vulkanStages(ub.inputDestination));
    }
    for (auto & s : _samplers) {
        _pipelineLayoutManager->addDescriptorPoolSize(vulkan::vk::DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ctx->swapchainSize());
        _pipelineLayoutManager->addBinding(s.binding, vulkan::vk::DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, vulkanStages(s.inputDestination));
    }
    _pipelineLayoutManager->setDescriptorPoolMaxSets(ctx->swapchainSize());
    
    _pipelineLayoutManager->create();
    for (size_t i = 0; i<ctx->swapchainSize(); ++i) {
        _descriptorSets.push_back(_pipelineLayoutManager->createDescriptorSet());
    }
}

void VulkanShaderInputs::addUniformBuffer(uint32_t binding, size_t bufferSize, uint32_t dest) {
    using namespace bg::engine::vulkan;
    UniformBufferData ubd;
    auto ctx = bg::engine::vulkan::context(context());
    BufferHelper helper(ctx->physicalDevice(), ctx->device());
    helper.createBuffer(
        bufferSize,
        vk::BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        vk::SHARING_MODE_EXCLUSIVE,
        vk::MEMORY_PROPERTY_HOST_VISIBLE_BIT | vk::MEMORY_PROPERTY_HOST_COHERENT_BIT,
        ubd.buffer, ubd.memory);
    ubd.binding = binding;
    ubd.inputDestination = dest;
    _uniformBuffers.push_back(ubd);
}

void VulkanShaderInputs::addSampler(uint32_t binding, uint32_t dest) {
    _samplers.push_back({
        binding,
        dest
    });
}

void VulkanShaderInputs::mapBufferMemory(uint32_t index, void ** data, size_t size) {
    _uniformBuffers[index].memory->mapMemory(0, size, 0, data);
}

void VulkanShaderInputs::unmapBufferMemory(uint32_t index) {
    _uniformBuffers[index].memory->unmapMemory();
}

}
}
