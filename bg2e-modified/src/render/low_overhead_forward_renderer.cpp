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


#include <bg/render/low_overhead_forward_renderer.hpp>

#include <bg/scene/drawable.hpp>
#include <bg/scene/light.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/tools/texture_cache.hpp>

#include <bg/wnd/vk_context.hpp>
#include <bg/wnd/window_controller.hpp>

#include <bg/log.hpp>

#include <bg/render/vulkan_low_overhead_pipeline.hpp>

namespace bg {
namespace render {


LowOverheadForwardRenderer::LowOverheadForwardRenderer(bg::base::Context * ctx)
    :Renderer(ctx)
{

}

LowOverheadForwardRenderer::~LowOverheadForwardRenderer() {
    bg::engine::vulkan::context(context())->device()->waitIdle();

    _renderManager = nullptr;
    _loPipeline = nullptr;
}
    
bool LowOverheadForwardRenderer::isSupported() {
    return bg::Engine::IsVulkan();
}
    
void LowOverheadForwardRenderer::create() {
    using namespace bg::engine::vulkan;

    _loPipeline = bg::render::LowOverheadPipeline::Create(context(), bg::render::LowOverheadPipeline::BlendOptions::AlphaBlend());
    _loPipeline->setOpacityLayer(LowOverheadPipeline::kOpacityAll);
    // TODO: Set default shader material if this._shaderMaterial.isNull()
    //_loPipeline->setShaderMaterial(_shaderMaterial.getPtr());
    
    _updateVisitor = new bg::render::UpdateVisitor();
    _transformVisitor = new bg::scene::TransformVisitor();
    
    _renderQueueVisitor = new bg::scene::RenderQueueVisitor();
}

void LowOverheadForwardRenderer::resize(int w, int h) {
    using namespace bg::engine::vulkan;
    vkContext()->resizeSwapchain();
    _renderManager = new RenderManager(vkContext());
    _renderManager->create();
    
    _loPipeline->resize(w, h);
}
    
void LowOverheadForwardRenderer::frame(bg::scene::Node * node, float delta) {
    _renderManager->waitForPreviousFrame();
    
    _frame->setDelta(delta);
    node->accept(_frame.getPtr());
    bg::scene::Node::FlushSceneChanges();
    
    _delta = delta;
}
    
void LowOverheadForwardRenderer::draw(bg::scene::Node * node, bg::scene::Camera * camera) {
    using namespace bg::engine;
    auto vkContext = vulkan::context(context());
    
    _transformVisitor->matrix().identity();
    camera->node()->acceptReverse(_transformVisitor.getPtr());
   
    if (_loPipeline->shaderMaterial()) {
        _loPipeline->shaderMaterial()->beginFrame(_delta);
    }
    
    // Update pipeline data
    _loPipeline->matrixState()->viewMatrixStack()
        .set(_transformVisitor->matrix())
        .invert();

    _loPipeline->matrixState()->projectionMatrixStack().set(camera->projection());
    
    _loPipeline->shaderMaterial()->setCamera(camera);

    auto currentFrame = _renderManager->currentFrame();
    _loPipeline->matrixState()->modelMatrixStack().identity();
    _updateVisitor->prepare(_loPipeline.getPtr(), currentFrame);
    node->accept(_updateVisitor.getPtr());
    
    _renderQueueVisitor->setModelMatrixStack(&_loPipeline->matrixState()->modelMatrixStack());
    _renderQueueVisitor->renderQueue().beginFrame(camera->worldPosition());
    node->accept(_renderQueueVisitor.getPtr());
    _renderQueueVisitor->renderQueue().sortTransparentObjects();
    
    // Record the command buffer for this frame
    vulkan::vk::CommandBuffer * buff = vkContext->commandBuffer(currentFrame);
    buff->resetCommandBuffer(vulkan::vk::COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    buff->beginCommandBuffer(vulkan::vk::COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
    vulkan::vk::Rect2D area;
    area.offset = { 0, 0 };
    area.extent = vkContext->swapchain()->extent();
    std::vector<vulkan::vk::ClearValue> clearValues;
    clearValues.push_back({ 0.1f,0.133f,0.287f,1.0f }); // Clear color value
    clearValues.push_back({ 1.0f, 0 });  // Clear depth and stencil values

    auto pl = _loPipeline->cast<bg::render::VulkanLowOverheadPipeline>()->enginePipeline();
    buff->cmdBeginRenderPass(pl->renderPass(), vkContext->framebuffer(currentFrame), area, clearValues, vulkan::vk::SUBPASS_CONTENTS_INLINE);
    buff->cmdBindPipeline(vulkan::vk::PIPELINE_BIND_POINT_GRAPHICS, pl);

    _loPipeline->matrixState()->modelMatrixStack().push();
    for (auto item : _renderQueueVisitor->renderQueue().opaqueQueue()) {
        _loPipeline->drawPolyList(item.plist, currentFrame);
    }
    _loPipeline->matrixState()->modelMatrixStack().pop();
    
    _loPipeline->matrixState()->modelMatrixStack().push();
    for (auto item : _renderQueueVisitor->renderQueue().transparentQueue()) {
        _loPipeline->drawPolyList(item.plist, currentFrame);
    }
    _loPipeline->matrixState()->modelMatrixStack().pop();

    buff->cmdEndRenderPass();
    buff->endCommandBuffer();
    
    _renderManager->presentNextFrame();
}

void LowOverheadForwardRenderer::invalidateTextures() {
}
    
    
}
}
