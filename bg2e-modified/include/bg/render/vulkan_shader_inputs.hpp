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


#ifndef _bg2e_render_vulkan_shader_input_hpp_
#define _bg2e_render_vulkan_shader_input_hpp_

#include <bg/render/shader_inputs.hpp>
#include <bg/engine/vulkan.hpp>

namespace bg {
namespace render {

class BG2E_EXPORT VulkanShaderInputs : public ShaderInputs {
public:
    VulkanShaderInputs(bg::base::Context *);

    virtual void createLayout();

    inline bg::engine::vulkan::PipelineLayoutManager * layoutManager() { return _pipelineLayoutManager.getPtr(); }
    inline const bg::engine::vulkan::PipelineLayoutManager * layoutManager() const { return _pipelineLayoutManager.getPtr(); }
    
    
    virtual void addUniformBuffer(uint32_t binding, size_t bufferSize, uint32_t shaderInputDestination);
    virtual void addSampler(uint32_t binding, uint32_t shaderInputDestination);
    virtual void mapBufferMemory(uint32_t index, void ** data, size_t size);
    virtual void unmapBufferMemory(uint32_t index);
    
    struct UniformBufferData {
        bg::ptr<bg::engine::vulkan::vk::Buffer> buffer;
        bg::ptr<bg::engine::vulkan::vk::DeviceMemory> memory;
        uint32_t binding;
        uint32_t inputDestination;
        
        inline void operator=(UniformBufferData & other) {
            buffer = other.buffer.getPtr();
            memory = other.memory.getPtr();
            binding = other.binding;
            inputDestination = other.inputDestination;
        }
    };
    
    struct SamplerData {
        uint32_t binding;
        uint32_t inputDestination;
        
        inline void operator=(const SamplerData & other) {
            binding = other.binding;
            inputDestination = other.inputDestination;
        };
    };
    
    inline std::vector<UniformBufferData> & uniformBuffers() { return _uniformBuffers; }
    inline const std::vector<UniformBufferData> & uniformBuffers() const { return _uniformBuffers; }
    inline std::vector<SamplerData> & samplers() { return _samplers; }
    inline const std::vector<SamplerData> & samplers() const { return _samplers; }
    inline const bg::engine::vulkan::vk::DescriptorSet * descriptorSet(uint32_t frameIndex) const { return _descriptorSets[frameIndex].getPtr(); }
    inline bg::engine::vulkan::vk::DescriptorSet * descriptorSet(uint32_t frameIndex) { return _descriptorSets[frameIndex].getPtr(); }
    inline const bg::engine::vulkan::vk::DescriptorSet * descriptorSet(size_t frameIndex) const { return _descriptorSets[frameIndex].getPtr(); }
    inline bg::engine::vulkan::vk::DescriptorSet * descriptorSet(size_t frameIndex) { return _descriptorSets[frameIndex].getPtr(); }
    
protected:
    virtual ~VulkanShaderInputs();
    
    bg::ptr<bg::engine::vulkan::PipelineLayoutManager> _pipelineLayoutManager;
    
    std::vector<UniformBufferData> _uniformBuffers;
    std::vector<SamplerData> _samplers;
    std::vector<bg::ptr<bg::engine::vulkan::vk::DescriptorSet>> _descriptorSets;
};


}
}

#endif
