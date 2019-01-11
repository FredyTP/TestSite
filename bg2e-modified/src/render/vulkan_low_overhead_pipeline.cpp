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


#include <bg/render/vulkan_low_overhead_pipeline.hpp>
#include <bg/render/vulkan_shader_inputs.hpp>

#include <bg/base/context.hpp>
#include <bg/wnd/context.hpp>
#include <bg/wnd/vk_context.hpp>
#include <bg/scene/camera.hpp>

#include <bg/log.hpp>

namespace bg {
namespace render {

inline bg::engine::vulkan::vk::BlendFactor vulkanBlendFactor(LowOverheadPipeline::BlendFactor f) {
    using namespace bg::engine::vulkan;
    switch (f) {
    case LowOverheadPipeline::kBlendFactorZero:
        return vk::BLEND_FACTOR_ZERO;
    case LowOverheadPipeline::kBlendFactorOne:
        return vk::BLEND_FACTOR_ONE;
    case LowOverheadPipeline::kBlendFactorSrcColor:
        return vk::BLEND_FACTOR_SRC_COLOR;
    case LowOverheadPipeline::kBlendFactorOneMinusSrcColor:
        return vk::BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case LowOverheadPipeline::kBlendFactorDstColor:
        return vk::BLEND_FACTOR_DST_COLOR;
    case LowOverheadPipeline::kBlendFactorOneMinusDstColor:
        return vk::BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case LowOverheadPipeline::kBlendFactorSrcAlpha:
        return vk::BLEND_FACTOR_SRC_ALPHA;
    case LowOverheadPipeline::kBlendFactorOneMinusSrcAlpha:
        return vk::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case LowOverheadPipeline::kBlendFactorDstAlpha:
        return vk::BLEND_FACTOR_DST_ALPHA;
    case LowOverheadPipeline::kBlendFactorOneMinusDstAlpha:
        return vk::BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case LowOverheadPipeline::kBlendFactorConstantColor:
        return vk::BLEND_FACTOR_CONSTANT_COLOR;
    case LowOverheadPipeline::kBlendFactorOneMinusConstantColor:
        return vk::BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case LowOverheadPipeline::kBlendFactorConstantAlpha:
        return vk::BLEND_FACTOR_CONSTANT_ALPHA;
    case LowOverheadPipeline::kBlendFactorOneMinusConstantAlpha:
        return vk::BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    case LowOverheadPipeline::kBlendFactorSrcAlphaSaturate:
        return vk::BLEND_FACTOR_SRC_ALPHA_SATURATE;
    default:
        return vk::BLEND_FACTOR_ZERO;
    }
}

inline bg::engine::vulkan::vk::BlendOp vulkanBlendOperation(LowOverheadPipeline::BlendOperation op) {
    using namespace bg::engine::vulkan;
    switch (op) {
    case LowOverheadPipeline::kBlendOpAdd:
        return vk::BLEND_OP_ADD;
    case LowOverheadPipeline::kBlendOpSubtract:
        return vk::BLEND_OP_SUBTRACT;
    case LowOverheadPipeline::kBlendOpReverseSubtract:
        return vk::BLEND_OP_REVERSE_SUBTRACT;
    case LowOverheadPipeline::kBlendOpMin:
        return vk::BLEND_OP_MIN;
    case LowOverheadPipeline::kBlendOpMax:
        return vk::BLEND_OP_MAX;
    default:
        return vk::BLEND_OP_ADD;
    }
}

VulkanLowOverheadPipeline::VulkanLowOverheadPipeline(bg::base::Context * ctx, const BlendOptions & opt)
    :LowOverheadPipeline(ctx, opt)
{
}

VulkanLowOverheadPipeline::~VulkanLowOverheadPipeline() {
    if (_shaderHelper) {
        delete _shaderHelper;
    }
}
    
void VulkanLowOverheadPipeline::resize(int w, int h) {
    using namespace bg::engine::vulkan;
    
    auto shaderInputData = _shaderInputManager->getShaderInput(nullptr);
    auto layout = dynamic_cast<bg::render::VulkanShaderInputs*>(shaderInputData->shaderInputs.getPtr())->layoutManager()->pipelineLayout();
    setPipelineLayout(layout);
    
    if (w == _width && h == _height) {
        bg::log(bg::log::kWarning) << "VulkanLowOverheadPipeline::resize(): command ignored, the viewport size is unchanged." << bg::endl;
        return;
    }
    
    _pipelinesCullFront.clear();
    _pipelinesCullBack.clear();
    _pipelinesCullOff.clear();
    
    createPipeline(w, h, kTopologyTriangleList, kCullModeBack);
    createPipeline(w, h, kTopologyLineList, kCullModeBack);
    createPipeline(w, h, kTopologyPointList, kCullModeBack);
    createPipeline(w, h, kTopologyTriangleList, kCullModeFront);
    createPipeline(w, h, kTopologyLineList, kCullModeFront);
    createPipeline(w, h, kTopologyPointList, kCullModeFront);
    createPipeline(w, h, kTopologyTriangleList, kCullModeOff);
    createPipeline(w, h, kTopologyLineList, kCullModeOff);
    createPipeline(w, h, kTopologyPointList, kCullModeOff);
}
    
void VulkanLowOverheadPipeline::check() {
}

void VulkanLowOverheadPipeline::mapInputBuffer(uint32_t bufferIndex, bg::base::PolyList * pl, void * memData, size_t size) {
    auto sid = shaderInputManager()->getShaderInput(pl);
    void * data;
    sid->shaderInputs->mapBufferMemory(bufferIndex, &data, size);
    memcpy(data,memData,size);
    sid->shaderInputs->unmapBufferMemory(bufferIndex);
}

void VulkanLowOverheadPipeline::drawPolyList(bg::base::PolyList * plist, uint32_t currentFrame) {
    auto ctx = bg::engine::vulkan::context(context());
    auto buff = ctx->commandBuffer(currentFrame);
    
    auto shaderInput = dynamic_cast<bg::render::VulkanShaderInputs*>(shaderInputManager()->getShaderInput(plist)->shaderInputs.getPtr());
    auto descSet = shaderInput->descriptorSet(currentFrame);
    auto plistImpl = plist->impl<bg::engine::vulkan::PolyListImpl>();

    buff->cmdBindVertexBuffers(0, 1, { plistImpl->vertexBuffer() }, { 0 });
    buff->cmdBindIndexBuffer(plistImpl->indexBuffer(), 0, bg::engine::vulkan::vk::INDEX_TYPE_UINT32);
    buff->cmdBindDescriptorSets(
        bg::engine::vulkan::vk::PIPELINE_BIND_POINT_GRAPHICS, shaderInput->layoutManager()->pipelineLayout(), 0,
        { descSet }, {}
    );
    buff->cmdDrawIndexed(plist->indexCount(), 1, 0, 0, 0);
}

void VulkanLowOverheadPipeline::destroy() {
    _pipelineLayout = nullptr;
    _pipelinesCullFront.clear();
    _pipelinesCullBack.clear();
    _pipelinesCullOff.clear();
}
    
void VulkanLowOverheadPipeline::createPipeline(
    int w, int h,
    PrimitiveTopology t,
    CullFaceMode cullMode)
{
    using namespace bg::engine::vulkan;
    auto vkCtx = bg::engine::vulkan::context(context());
    vk::Pipeline * pl = new vk::Pipeline();
    
    for (auto & shader : _shaders) {
        addShader(pl, shader);
    }
    
    pl->vertexInputState.vertexBindingDescriptions.push_back(getVertexInputBindingDescription<VertexPT0T1NC>(0));
    pl->vertexInputState.vertexAttributeDescriptions = getAttributeDescriptions<VertexPT0T1NC>(0);
    
    switch (t) {
    case kTopologyTriangleList:
        pl->inputAssemblyState.topology = vk::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        break;
    case kTopologyPointList:
        pl->inputAssemblyState.topology = vk::PRIMITIVE_TOPOLOGY_POINT_LIST;
        break;
    case kTopologyLineList:
        pl->inputAssemblyState.topology = vk::PRIMITIVE_TOPOLOGY_LINE_LIST;
        break;
    }
    pl->inputAssemblyState.primitiveRestartEnable = false;
    
    vk::Viewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(w);
    viewport.height = static_cast<float>(h);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vk::Rect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = vkCtx->swapchain()->extent();
   
    pl->setPipelineLayout(_pipelineLayout.getPtr());
    
    pl->viewportState.viewports.push_back(viewport);
    pl->viewportState.scissors.push_back(scissor);
    
    switch (cullMode) {
    case kCullModeFront:
        pl->rasterizationState.cullMode = vk::CULL_MODE_FRONT_BIT;
        break;
    case kCullModeBack:
        pl->rasterizationState.cullMode = vk::CULL_MODE_BACK_BIT;
        break;
    case kCullModeOff:
        pl->rasterizationState.cullMode = vk::CULL_MODE_NONE;
        break;
    }
    pl->rasterizationState.frontFace = vk::FRONT_FACE_COUNTER_CLOCKWISE;
    
    // TODO: vk::DYNAMIC_STATE_BLEND_CONSTANTS
    pl->depthStencilState.depthTestEnable = true;
    pl->depthStencilState.depthWriteEnable = true;
    pl->depthStencilState.depthCompareOp = vk::COMPARE_OP_LESS;
    pl->depthStencilState.depthBoundsTestEnable = false;
    pl->depthStencilState.minDepthBounds = 0.0f;
    pl->depthStencilState.maxDepthBounds = 1.0f;
    pl->depthStencilState.stencilTestEnable = false;
    
    vk::PipelineColorBlendAttachmentState blendState = {};
    blendState.blendEnable = _blendOptions.blendEnabled;
    blendState.srcColorBlendFactor = vulkanBlendFactor(_blendOptions.srcColorBlendFactor);
    blendState.dstColorBlendFactor = vulkanBlendFactor(_blendOptions.dstColorBlendFactor);
    blendState.colorBlendOp = vulkanBlendOperation(_blendOptions.colorBlendOp);
    blendState.srcAlphaBlendFactor = vulkanBlendFactor(_blendOptions.srcAlphaBlendFactor);
    blendState.dstAlphaBlendFactor = vulkanBlendFactor(_blendOptions.dstAlphaBlendFactor);
    blendState.alphaBlendOp = vulkanBlendOperation(_blendOptions.alphaBlendOp);
    blendState.colorWriteMask = vk::COLOR_COMPONENT_R_BIT |
                                vk::COLOR_COMPONENT_G_BIT |
                                vk::COLOR_COMPONENT_B_BIT |
                                vk::COLOR_COMPONENT_A_BIT;
    pl->colorBlendState.attachments.push_back(blendState);

    pl->setRenderPass(vkCtx->renderPass());
    
    pl->create(vkCtx->device());
    
    switch (cullMode) {
    case kCullModeFront:
        _pipelinesCullFront[t] = pl;
        break;
    case kCullModeBack:
        _pipelinesCullBack[t] = pl;
        break;
    case kCullModeOff:
        _pipelinesCullOff[t] = pl;
        break;
    }
}
    
void VulkanLowOverheadPipeline::addShader(bg::engine::vulkan::vk::Pipeline * pl, const ShaderModuleData & data) {
    using namespace bg::engine::vulkan;
    if (!_shaderHelper) {
        _shaderHelper = new ShaderHelper(bg::engine::vulkan::context(context())->device());
    }
    bg::ptr<vk::ShaderModule> shader = _shaderHelper->loadFromFile(data.shaderPath);
    vk::ShaderStageFlagBits vkType;
    switch (data.shaderType) {
    case kShaderStageVertex:
        vkType = vk::SHADER_STAGE_VERTEX_BIT;
        break;
    case kShaderStageFragment:
        vkType = vk::SHADER_STAGE_FRAGMENT_BIT;
        break;
    }
    pl->shaderStages.push_back({
        shader.getPtr(),
        data.entryPoint,
        vkType
    });
}

}
}
