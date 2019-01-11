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


#ifndef _bg2e_render_vulkan_low_overhead_pipeline_hpp_
#define _bg2e_render_vulkan_low_overhead_pipeline_hpp_

#include <bg/render/low_overhead_pipeline.hpp>
#include <bg/engine/vulkan.hpp>

#include <bg/wnd/window_controller.hpp>

namespace bg {
namespace render {

class BG2E_EXPORT VulkanLowOverheadPipeline : public LowOverheadPipeline {
public:
    VulkanLowOverheadPipeline(bg::base::Context *, const BlendOptions &);

    virtual void resize(int w, int h);
    virtual void check();
    virtual void mapInputBuffer(uint32_t bufferIndex, bg::base::PolyList * pl, void * memData, size_t size);
    virtual void drawPolyList(bg::base::PolyList * plist, uint32_t currentFrame);

    inline bg::engine::vulkan::vk::Pipeline * enginePipeline() {
        using namespace bg::engine::vulkan;
        return _cullMode == kCullModeBack  ? _pipelinesCullBack[_primitiveTopology].getPtr() :
               _cullMode == kCullModeFront ? _pipelinesCullFront[_primitiveTopology].getPtr() :
                                             _pipelinesCullOff[_primitiveTopology].getPtr();
    }
    
    inline void setPipelineLayout(bg::engine::vulkan::vk::PipelineLayout * lo) { _pipelineLayout = lo; }
    
    virtual void destroy();
    
protected:
    virtual ~VulkanLowOverheadPipeline();
    
    void createPipeline(int w, int h, PrimitiveTopology t, CullFaceMode cullMode);
    
    std::unordered_map<int32_t, bg::ptr<bg::engine::vulkan::vk::Pipeline>> _pipelinesCullFront;
    std::unordered_map<int32_t, bg::ptr<bg::engine::vulkan::vk::Pipeline>> _pipelinesCullBack;
    std::unordered_map<int32_t, bg::ptr<bg::engine::vulkan::vk::Pipeline>> _pipelinesCullOff;
    
    bg::ptr<bg::engine::vulkan::vk::PipelineLayout> _pipelineLayout;
    bg::engine::vulkan::ShaderHelper * _shaderHelper = nullptr;
    uint32_t _width = 0;
    uint32_t _height = 0;
    
    void addShader(bg::engine::vulkan::vk::Pipeline * pl, const ShaderModuleData & data);
};

}
}

#endif
