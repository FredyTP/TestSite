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


#include <bg/render/low_overhead_pipeline.hpp>
#include <bg/engine/vulkan.hpp>

#include <bg/render/vulkan_low_overhead_pipeline.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace render {

LowOverheadPipeline::LowOverheadPipeline(bg::base::Context * ctx, const BlendOptions & opt)
    :bg::base::ContextObject(ctx)
    ,_primitiveTopology(kTopologyTriangleList)
    ,_cullMode(kCullModeBack)
    ,_opacityLayer(kOpacityAll)
    ,_blendOptions(opt)
{
    _shaderInputManager = new ShaderInputManager(ctx);
    _matrixState = new bg::base::MatrixState();
}
    
LowOverheadPipeline * LowOverheadPipeline::Create(bg::base::Context * ctx, const BlendOptions & opt) {
    if (bg::Engine::Get()->IsVulkan()) {
        return new VulkanLowOverheadPipeline(ctx, opt);
    }
    else {
        throw new bg::base::CompatibilityException("The current rendering engine is not compatible with low overhead pipelines");
    }
}

void LowOverheadPipeline::mapInputBuffer(bg::base::PolyList * pl, bg::base::Material * mat) {
    if (_shaderMaterial.valid()) {
        _shaderMaterial->mapInputBuffers(pl, mat);
    }
}

void LowOverheadPipeline::updateShaderInputs(bg::base::PolyList * pl, bg::base::Material * mat, uint32_t currentFrame) {
    if (_shaderMaterial.valid()) {
        _shaderMaterial->updateDescriptorSet(pl, mat, currentFrame);
    }
}


}
}
